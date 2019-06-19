#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headers/create_server.h"
#include "headers/errors.h"
#define BUFFER_SIZE 256
#define SERVER_ARGUMENTS_TEMPLATE "<PORT>"
void *handle_connection(void *uncasted_socket_pointer) {
  char buffer[BUFFER_SIZE] = {0};
  int valread;
  int socket = *((int *)uncasted_socket_pointer);
  printf("Connected!\n");

  while (1) {
    valread = read(socket, buffer, BUFFER_SIZE - 1);
    if (valread > 0) {
      buffer[valread] = 0;
      printf(buffer);
    } else {
      printf("Disconnected!\n");
      return 0;
    }
  }
}


int main(int argc, char *argv[]) {
  if (argc < 2)
    usage(argv[0], SERVER_ARGUMENTS_TEMPLATE);
  ServerData connection ={atoi(argv[1]), handle_connection};
  create_server(&connection);
  return 0;
}
