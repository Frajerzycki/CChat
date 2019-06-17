#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "headers/errors.h"
#define PORT 8080
#define BUFFER_SIZE 256 
void* handle_connection(void* socket_pointer) {
    char buffer[BUFFER_SIZE] = {0};
    int valread;
    int socket = *((int*) socket_pointer);
    printf("Connected!\n");
    while(1) {
        valread = read(socket, buffer, BUFFER_SIZE-1);
        if (valread > 0) {
            buffer[valread] = 0;
            printf(buffer);
        }
    }   
}
int main(int argc, char *argv[]) {
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);  
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
        show_error_message_and_exit(); 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt)))
        show_error_message_and_exit();
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
        show_error_message_and_exit();
    while (1) {    
        if (listen(server_fd, 3) < 0) 
            show_error_message_and_exit(); 
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                               (socklen_t*)&addrlen))>=0) 
        {
            pthread_t thread_id; 
            pthread_create(&thread_id, NULL, handle_connection, &new_socket);
        }    
    }    
    return 0; 
} 
