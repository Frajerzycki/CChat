#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include "headers/connection.h"
#include "headers/errors.h"

void *create_server(void *uncasted_connection_pointer) {
  Connection *connection = (Connection *)uncasted_connection_pointer;
  int opt = 1, fd, new_socket;
  struct sockaddr_in address;
  socklen_t address_len = sizeof(address);
  // Creating socket file descriptor
  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    show_error_message_and_exit();

  // Forcefully attaching socket to the port 8080
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt)))
    show_error_message_and_exit();
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(connection->port);

  struct sockaddr *casted_address = (struct sockaddr *)&address;
  // Forcefully attaching socket to the port 8080
  if (bind(fd, casted_address, address_len) < 0)
    show_error_message_and_exit();
  do {
    if (listen(fd, 3) < 0)
      show_error_message_and_exit();
    if ((new_socket = accept(fd, casted_address, &address_len)) >= 0)
      connection->handle(new_socket);
  } while (connection->repeat);
  return NULL;
}

void async_create_server(void *uncasted_connection_pointer) {
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, create_server, uncasted_connection_pointer);
}

void *create_client(void *uncasted_connection_pointer) {
  Connection *connection = (Connection *)uncasted_connection_pointer;
  int new_socket;
  struct sockaddr_in serv_addr;
  if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    show_error_message_and_exit();

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(connection->port);

  // Convert ipv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, connection->ip, &serv_addr.sin_addr) <= 0)
    show_error_message_and_exit();

  if (connect(new_socket, (struct sockaddr*)&serv_addr,
              sizeof(serv_addr)) < 0)
    show_error_message_and_exit();
  do
    connection->handle(new_socket);
  while (connection->repeat);
  return NULL;
}
