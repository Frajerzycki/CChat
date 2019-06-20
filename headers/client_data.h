
// Contains data necessary to create client.
typedef struct {
    char* ip;
    int port;
    void (*send_message)(int);
    void* (*receive_messages)(void*);
} ClientData;
