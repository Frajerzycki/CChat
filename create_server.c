#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <string.h>
#include "headers/server_data.h"
#include "headers/errors.h"

void *create_server(void *uncasted_data_pointer) {
  ServerData *data = (ServerData *)uncasted_data_pointer;
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
  address.sin_port = htons(data->port);

  struct sockaddr *casted_address = (struct sockaddr *)&address;
  // Forcefully attaching socket to the port 8080
  if (bind(fd, casted_address, address_len) < 0)
    show_error_message_and_exit();
  while (1) {
    if (listen(fd, 3) < 0)
      show_error_message_and_exit();
    if ((new_socket = accept(fd, casted_address, &address_len)) >= 0) {
      pthread_t thread_id;
      pthread_create(&thread_id, NULL, data->handle, &new_socket);
    }
  }
  return NULL;
}
