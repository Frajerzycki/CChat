#include "headers/errors.h"
#include "headers/sizes.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define CLIENT_ARGUMENTS_TEMPLATE "<IP:PORT>"
void parse_arguments(char *argv[], char **ip, int *port) {
  unsigned int i, first_argument_length = strlen(argv[1]);
  for (i = 0; i < first_argument_length && argv[1][i] != ':'; i++)
    ;
  if (argv[1][i] != ':')
    usage(argv[0], CLIENT_ARGUMENTS_TEMPLATE);
  *ip = argv[1];
  (*ip)[i] = 0;
  *port = atoi(*ip + i + 1);
}

void send_messages(int socket) {
  char message[BUFFER_SIZE + 1];
  while (1) {
    fgets(message, BUFFER_SIZE, stdin);
    send(socket, message, strlen(message), 0);
  }
}

void *receive_messages(void *socket_pointer) {
  char buffer[BUFFER_SIZE + 1];
  int socket = *((int *)socket_pointer), valread;
  while (1) {
    valread = read(socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
      buffer[valread] = 0;
      printf("%s", buffer);
    } else
      show_error_message_and_exit("Disconnected!");
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    usage(argv[0], CLIENT_ARGUMENTS_TEMPLATE);
  int port, new_socket;
  char *ip;
  struct sockaddr_in serv_addr;
  parse_arguments(argv, &ip, &port);
  if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    show_errno_and_exit();

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port);

  // Convert ipv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    show_errno_and_exit();

  if (connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    show_errno_and_exit();
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, receive_messages, (void *)&new_socket);
  send_messages(new_socket);
  return 0;
}
