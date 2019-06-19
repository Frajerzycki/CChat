
// Contains data necessary to create server.
typedef struct {
    int port;
    void *(*handle)(void*);
} ServerData;

