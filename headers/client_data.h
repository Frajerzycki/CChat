
// Contains data necessary to create client.
typedef struct {
    char* ip;
    int port;
    void (*handle)(int);
} ClientData;
