#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "headers/client_data.h"
#include "headers/errors.h"


void create_client(ClientData *data) {
  int new_socket;
  struct sockaddr_in serv_addr;
  if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    show_error_message_and_exit();

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(data->port);

  // Convert ipv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, data->ip, &serv_addr.sin_addr) <= 0)
    show_error_message_and_exit();

  if (connect(new_socket, (struct sockaddr*)&serv_addr,
              sizeof(serv_addr)) < 0)
    show_error_message_and_exit();
  
  while (1)
    data->handle(new_socket);
}
