#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "headers/create_client.h"
#include "headers/create_server.h"
#include "headers/errors.h"
#define BUFFER_SIZE 256
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

void send_message(int socket) {
  char message[200];
  fgets(message, 199, stdin);
  send(socket, message, strlen(message), 0);
}

void* receive_messages(void* socket_pointer) {
    char buffer[BUFFER_SIZE+1];
    int socket = *((int*)socket_pointer), valread;
    while (1) {
        valread = read(socket, buffer, BUFFER_SIZE);
        if (valread > 0) {
          buffer[valread] = 0;
          printf("%s",buffer);
        } else {
          printf("Disconnected!\n");
          exit(0); 
        }
    }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    usage(argv[0], CLIENT_ARGUMENTS_TEMPLATE);
  ClientData *data = malloc(sizeof(ClientData));
  data->send_message = send_message;
  data->receive_messages = receive_messages;
  parse_arguments(argv, &data->ip, &data->port);
  create_client(data);
  return 0;
}
