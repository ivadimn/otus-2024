#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_NAME "filesize.socket"
#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

int run_server(const char* cmd, const char* filename); 
int64_t get_file_size(const char* filename, char* err_msg);
int handle_query(int client_socket, char* query, const char* filename);

#endif