#include "headers/errors.h"
#include "headers/connection.h"
#include "headers/sizes.h"
#include <arpa/inet.h> //close
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //strlen
#include <sys/socket.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <sys/types.h>
#include <unistd.h> //close
#define SERVER_ARGUMENTS_TEMPLATE "<PORT> <MAX CLIENTS>"
int max_clients;
void initialize_clients(connection *clients[]) {
  for (unsigned int i = 0; i < max_clients; i++) {
    clients[i] = malloc(sizeof(connection));
    clients[i]->sd = 0;
  }
}

void send_message_to_all_clients(char message[], connection *clients[],
                               int number_of_sender) {
  for (unsigned int i = 0; i < max_clients; i++) {
    int sdi = clients[i]->sd;
    if (sdi > 0 && number_of_sender != i)
      send(sdi, message, strlen(message), 0);
  }
}



int main(int argc, char *argv[]) {
  if (argc < 3)
    usage(argv[0], SERVER_ARGUMENTS_TEMPLATE);
  int port = atoi(argv[1]);
  if (port < 1 || port > 65535)
    show_error_message_and_exit("Port have to be in bounds <1,65535>.");
  max_clients = atoi(argv[2]);
  if (max_clients < 1)
    show_error_message_and_exit(
        "Number of maximum clients should be greater than 0.");
  connection *clients[max_clients];

  initialize_clients(clients);
  // create a master socket
  int master_socket;
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    show_errno_and_exit();

  int opt = 1;
  // set master socket to allow multiple connections
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                 sizeof(opt)) < 0)
    show_errno_and_exit();

  struct sockaddr_in address;
  // type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // bind the socket to localhost port 8888
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    show_errno_and_exit();

  // try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0)
    show_errno_and_exit();
  // accept the incoming connection
  size_t addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while (1) {
    fd_set readfds;
    // clear the socket set
    FD_ZERO(&readfds);

    // add master socket to set
    FD_SET(master_socket, &readfds);
    unsigned int max_sd = master_socket;
    // add child sockets to set
    for (unsigned int i = 0; i < max_clients; i++) {
      // socket descriptor
      unsigned int sdi = clients[i]->sd;

      // if valid socket descriptor then add to read list
      if (sdi > 0)
        FD_SET(sdi, &readfds);

      // highest file descriptor number, need it for the select function
      if (sdi > max_sd)
        max_sd = sdi;
    }

    // wait for an activity on one of the sockets , timeout is NULL ,
    // so wait indefinitely
    int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if (activity < 0 && errno != EINTR)
      printf("Error while select function.\n");

    // If something happened on the master socket ,
    // then its an incoming connection
    if (FD_ISSET(master_socket, &readfds)) {
      int new_socket;
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address,
                               (socklen_t *)&addrlen)) < 0)
        show_errno_and_exit();

      // inform connection of socket number - used in send and receive commands
      printf("New connection from %s:%d\n", inet_ntoa(address.sin_addr),
             ntohs(address.sin_port));
      unsigned int i = 0;
      while (i < max_clients && clients[i]->sd != 0)
        i++;
        // if position is empty
      if (i < max_clients)
        clients[i]->sd = new_socket;
      else
        close(new_socket);
    }

    // else its some IO operation on some other socket
    for (unsigned int i = 0; i < max_clients; i++) {
      unsigned int sdi = clients[i]->sd;

      if (FD_ISSET(sdi, &readfds)) {
        // Check if it was for closing , and also read the
        // incoming message
        char buffer[BUFFER_SIZE];
        ssize_t valread;
        if ((valread = read(sdi, buffer, BUFFER_SIZE - 1)) == 0) {
          // Somebody disconnected , get his details and print
          getpeername(sdi, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          printf("Host disconnected %s:%d\n", inet_ntoa(address.sin_addr),
                 ntohs(address.sin_port));

          // Close the socket and mark as 0 in list for reuse
          close(sdi);
          clients[i]->sd = 0;
        } else {
          buffer[valread] = 0;
          send_message_to_all_clients(buffer, clients, i);
        }
      }
    }
  }

  return 0;
}
