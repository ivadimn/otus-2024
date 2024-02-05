#include "common.h"
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>

static void err_doit(int, int, const char*, va_list);

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
* Обрабатывает нефатальные ошибки, связанные с системными вызовами
* Код ошибки передаётся в аргументе
* Выводит сообщение и возвращает управление
*/
void err_cont(const char *fmt, ...) {

    va_list ap;

    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
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

int64_t get_file_size(int fd) {

	int64_t fsize = 0;
	struct stat fileStatbuff;
	if ((fstat(fd, & fileStatbuff) != 0) || (!S_ISREG(fileStatbuff.st_mode))) {
		fsize = -1;
	}
	else{
		fsize = fileStatbuff.st_size;
	}
	return fsize;
}

void* alloc(size_t size) {
    void *p = malloc(size);
    if (p == NULL)
        err_sys("Ошибка распределения памяти: ");
    return p;
}
