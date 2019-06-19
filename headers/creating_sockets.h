// Creates server which handles connections in specified way by connection->handle function.
void create_server(void* uncasted_connection_pointer);
// Calls create_server in new thread.
void async_create_server(void* uncasted_connection_pointer);
// Creates client which handles connection in specified way by connection->handle function;
void create_client(void* uncasted_connection_pointer);
