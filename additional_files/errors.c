#include <stdio.h>
#include <stdlib.h>
void show_error_message_and_exit() {
    perror(NULL);
    exit(EXIT_FAILURE);
}
