#include "log.h"
#include "server.h"
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

char buf[1024];
char ans[1024];
char err[1024];

const char* help_msg = "Добро пожаловать!\n\
принимаемые команды:\n\
\tsize - вывести размер файла,\n\
\texit - закрыть сервер.\n";

const char* commands[] = {
    "size",
    "exit"
};

int handle_query(int client_socket, char* query, const char* filename) {
    
    int64_t fsize = 0;
    bzero(ans, 1024);
    size_t len = strlen(query);
    if (query[len-1] == '\n')
        query[len-1] = '\0';

    if(strcmp(query, commands[1]) == 0)
        return 1;
    if (strcmp(query, commands[0]) != 0)     {
        strcpy(ans, "Команда не поддерживается,\nподдерживаемые команды: size, exit.\n");
        send(client_socket, ans, strlen(ans), 0);
        return -1;
    }
    else {
        fsize = get_file_size(filename, err);
        if (fsize >= 0) {
            sprintf(ans, "Размер файла %s: %lu байт.\n", filename,  fsize);
            send(client_socket, ans, strlen(ans), 0);
            return 0;
        }
        else {
            send(client_socket, err, strlen(err), 0);
            return -1;
        }
        
    }
}

int run_server(const char* cmd, const char* filename) {

    int sock, client_sock, rval, rc = -1;
    struct sockaddr_un server;
    

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    log_info("Начало работы сервера %s\n", cmd);
    if (sock < 0) 
        log_sys("Ошибка при открытии сокета: %s", SOCKET_NAME);
    

    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, SOCKET_NAME);

    if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
        log_sys("Ошибка привязки сокета: %s", SOCKET_NAME);
    }

    log_info("Сокет имеет имя %s, %s\n", server.sun_path, cmd);
    
    listen(sock, 5);

    for (;;) {
        client_sock = accept(sock, 0, 0);
        if (client_sock == -1)
        {
            log_ret("Ошибка вызова функции accept.");
            break;
        }
        else  {
            send(client_sock, help_msg, strlen(help_msg), 0);
            do {
            bzero(buf, sizeof(buf));
            
                if ((rval = read(client_sock, buf, 1024)) < 0)
                    log_ret("Ошибка чтания данных из сокета.");
                else if (rval == 0)
                    log_ret("Закрытие соединения.");
                else {
                    rc = handle_query(client_sock, buf, filename);
                    if (rc >= 0)
                        break;
            }    
            } while (rval > 0);
        }

        close(client_sock);
        if (rc == 1)
            break;
        
    }
    close(sock);
    unlink(SOCKET_NAME);
    log_info("Окончание работы сервера %s\n", cmd);
    return 0;
}


int64_t get_file_size(const char* filename, char* err_msg) {
    
	int fd;
    int64_t fsize = 0;
	struct stat fileStatbuff;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        sprintf(err_msg, "Ошибка открытия файла %s: %s", filename,  strerror(errno));
        log_ret("Ошибка открытия файла: %s", filename);
        return -1;
    }
    if((fstat(fd, &fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
        sprintf(err_msg, "Ошибка обработки файла %s: %s", filename, strerror(errno));
        log_ret("Ошибка обработки файла: %s", filename);
		fsize = -1;
	}
	else{
		fsize = fileStatbuff.st_size;
	}
    close(fd);
	return fsize;
}

 

