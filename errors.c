#include <stdio.h>
#include <stdlib.h>
void show_error_message_and_exit() {
  perror(NULL);
  exit(EXIT_FAILURE);
}

void usage(char *filename, char *arguments_template) {
  printf("Usage:\t%s %s\n", filename, arguments_template);
  exit(0);
}
