#include "headers/creating_sockets.h"
#include "headers/errors.h"
#include "headers/connection.h"
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

void handle_connection(int socket) {
  char message[200];
  printf("Enter message ->\t");
  fgets(message, 199, stdin);
  printf(message);
  send(socket, message, strlen(message), 0);
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    usage(argv[0], CLIENT_ARGUMENTS_TEMPLATE);
  Connection *connection = malloc(sizeof(Connection));
  connection->repeat = 1;
  connection->handle = handle_connection;
  parse_arguments(argv, &connection->ip, &connection->port);
  create_client(connection);
  return 0;
}
