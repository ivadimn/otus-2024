#include "log.h"
#include <errno.h>
#include <syslog.h>
#include <stdarg.h>

static void err_doit(int, int, const char*, va_list);
static void log_paste(int errnoflag, int error, int priority, 
                    const char* fmt, va_list va);

extern int log_to_stderr;

/*
* Обрабатывает нефатальные ошибки, связанные с системными вызовами
* Выводит сообщение и возвращает управление 
*/
void err_ret(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение и завершает работу процесса
*/
void err_sys(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    exit(1);
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void err_cont(int error, const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void err_exit(int error, const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение, создает файл core и завершает работу процесса
*/
void err_dump(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    abort();                   // записать дамп памяти в файл и завершить процесс
    exit(1);                   // это не обязательно
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и возвращает управление 
*/
void err_msg(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и завершает работу процесса 
*/
void err_quit(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}

/*
* Выводит сообщение и возвращает управление в вызывающую функцию.
* Вызывающая функция определяет значение флага errnoflag.
*/
void err_doit(int errnoflag, int error, const char *fmt, va_list ap) {
    
    char buf[BUF_SIZE];

    vsnprintf(buf, BUF_SIZE-1, fmt, ap);
    if (errnoflag)     {
        snprintf(buf + strlen(buf), BUF_SIZE - strlen(buf) - 1, ": %s",
                strerror(error));
    }
    strcat(buf, "\n");
    fflush(stdout);
    fputs(buf, stderr);
    fflush(NULL);
}

void log_quit(const char*, ...) __attribute__((noreturn));

void log_exit(int, const char*, ...) __attribute__((noreturn));

/*
* инициализировать syslog если процесс работает в режиме демона
*/

void log_open(const char* app, int option, int facility)
{
    if(log_to_stderr == 0)
        openlog(app, option, facility);
}

/*
* Обрабатывает нефатальные ошибки, связанные с системными вызовами
* Выводит сообщение, соответствующее содержимому переменной errno
* и возвращает управление 
*/
void log_ret(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, errno, LOG_ERR, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, связанные с системными вызовами
* Выводит сообщение и завершает работу процесса
*/
void log_sys(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, errno, LOG_ERR, fmt, ap);
    exit(2);
}

/*
* Выводит информационное сообщение и возвращает управление 
*/
void log_info(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(0, 0, LOG_INFO, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает нефатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и возвращает управление 
*/
void log_msg(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(0, 0, LOG_ERR, fmt, ap);
    va_end(ap);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Выводит сообщение и завершает работу процесса 
*/
void log_quit(const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(0, 0, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(2);
}

/*
* Обрабатывает фатальные ошибки, не связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void log_exit(int error, const char *fmt, ...) {
    
    va_list ap;

    va_start(ap, fmt);
    log_paste(1, error, LOG_ERR, fmt, ap);
    va_end(ap);
    exit(2);
}


static void log_paste(int errnoflag, int error, int priority, 
                    const char* fmt, va_list ap) {

    char buf[BUF_SIZE];

    vsnprintf(buf, BUF_SIZE - 1, fmt, ap);
    if (errnoflag) {
        snprintf(buf + strlen(buf), BUF_SIZE - strlen(buf) - 1, ": %s", 
                strerror(error));
    }
    strcat(buf, "\n");
    if (log_to_stderr) {
        fflush(stdout);
        fputs(buf, stderr);
        fflush(stderr);
    } else {
        syslog(priority, "%s", buf);
    }
}
