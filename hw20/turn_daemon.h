#ifndef _TURN_DAEMON_H_
#define _TURN_DAEMON_H_

#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>


#define TD_NO_CHDIR             01  //не выполнять chdir("/")
#define TD_NO_CLOSE_FILES       02  //не закрывать все открытые файлы
#define TD_NO_REOPEN_STD_FDS    04  //не перенаправлять потоки stdin, stdout,
                                    //stderr в /dev/null
#define TD_NO_UMASK0            010  //не выполнять umask(0)

#define TD_MAX_CLOSE            1024  //максимальное количество закрываемых 
                                      //файловых дескрипторов, если операция
                                      //sysconf(_SC_OPEN_MAX) не определена
typedef struct sigaction sa_t;

int turn_daemon(const char* cmd);                                      

#endif