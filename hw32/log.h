#ifndef _LOG_H_
#define _LOG_H_

#define  _XOPEN_SOURCE 700

#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <execinfo.h>

#define RESET    "\033[0m"
#define DEBUG    "\033[1;33m"
#define INFO     "\033[1;37m"
#define ERROR    "\033[1;31m"
#define WARNING  "\033[1;36m"
#define CRIT     "\033[1;35m"

#define MAX_LEN 4096
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void err_msg(const char*, ...);
void err_dump(const char*, ...) __attribute__((noreturn));
void err_quit(const char*, ...) __attribute__((noreturn));
void err_cont(int, const char*, ...);
void err_exit(int, const char*, ...) __attribute__((noreturn));
void err_ret(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));


#define info(fmt, ...) logv(__func__, LOG_INFO, fmt __VA_OPT__(,) __VA_ARGS__)
#define debug(fmt, ...) logv(__func__, LOG_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)
#define err(fmt, ...) logv(__func__, LOG_ERR, fmt __VA_OPT__(,) __VA_ARGS__)
#define warning(fmt, ...) logv(__func__, LOG_WARNING, fmt __VA_OPT__(,) __VA_ARGS__)
#define crit(fmt, ...) logv(__func__, LOG_CRIT, fmt __VA_OPT__(,) __VA_ARGS__)

void log_open(const char*);
void log_close();
void logv(const char* func_name, int level, const char* fmt, ...);

#endif


