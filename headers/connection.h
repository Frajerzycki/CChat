typedef struct {
    char* ip;
    int port;
    int repeat;
    void (*handle)(int);
} Connection;
