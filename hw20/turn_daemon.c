#include "log.h"
#include "turn_daemon.h"
#include <stdlib.h>
#include <errno.h>

int turn_daemon(const char* cmd) {
    int fd;
    struct rlimit rl;
    struct sigaction sa;
    
    
    log_open(cmd, LOG_CONS, LOG_DAEMON);     //инициализация файла журнала
    
    /*
    * Получить максимально возможный номер дескриптора файла
    */
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
        log_quit("Невозможно получить максимальный номер дескриптора %s.", cmd);
    
    switch (fork())         //превращение в фоновый процесс 
    {
        case -1: log_sys("Ошибка вызова функции fork %s.", cmd);
        case 0:  break;                 //потомок проходит этот этап
        default: _exit(EXIT_SUCCESS);   //родитель завершается
    }
    log_info("Прошли первый fork %s.", cmd);
    
    if (setsid() == -1)                 //процесс становится лидером новой сессии
        log_sys("Не удалось стать лидером новой сессии %s.", cmd);

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        log_sys("Невозможно игнорировать сигнадл SIGHUP %s.", cmd);
    }

    switch (fork())                     //делаем так, чтобы процесс
                                        //не стал лидером сессии 
    {
        case -1: {
            log_sys("Ошибка вызова функции fork %s.", cmd);
        }
        case 0:  break;                 
        default: _exit(EXIT_SUCCESS);   
    }

    umask(0);                    //сбрасываем маску режима создания файлов
    
    //if (chdir("/") < 0)          //переходим в корневой каталог
    //    log_ret("Невозможно сделать текущим рабочим каталогом %s.", cmd);
    
    
    if (rl.rlim_max == RLIM_INFINITY)
            rl.rlim_max = TD_MAX_CLOSE;
    for (rlim_t i = 0; i < rl.rlim_max; i++)
        close(i);

    close(STDIN_FILENO);            //перенаправляем стандартные потоки
                                    //данных в /dev/null
    fd = open("/dev/null", O_RDWR);
    
    if (fd != STDIN_FILENO)
        log_sys("Ошибка открытия /dev/null %s.", cmd);

    if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
        log_sys("Ошибка дублирования потока STDOUT %s.", cmd);

    if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
        log_sys("Ошибка дублирования потока STDERR %s.", cmd);

    log_info("Мы в демоне %s.", cmd);
    return 0;
}

