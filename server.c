#include "headers/connection.h"
#include "headers/creating_sockets.h"
#include "headers/errors.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 256
#define SERVER_ARGUMENTS_TEMPLATE "<PORT>"
void *handle_connection(void *socket_pointer) {
  char buffer[BUFFER_SIZE] = {0};
  int valread;
  int socket = *((int *)socket_pointer);
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

void async_handle_connection(int socket) {
  pthread_t thread_id;
  pthread_create(&thread_id, NULL, handle_connection, &socket);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    usage(argv[0], SERVER_ARGUMENTS_TEMPLATE);
  Connection connection ={NULL, atoi(argv[1]), 1, async_handle_connection};
  create_server(&connection);
  return 0;
}
