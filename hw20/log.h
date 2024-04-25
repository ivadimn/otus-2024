#ifndef _LOG_H_
#define _LOG_H_

#define  _XOPEN_SOURCE 700

#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>


#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define BUF_SIZE 4096

void err_msg(const char*, ...);
void err_dump(const char*, ...) __attribute__((noreturn));
void err_quit(const char*, ...) __attribute__((noreturn));
void err_cont(int, const char*, ...);
void err_exit(int, const char*, ...) __attribute__((noreturn));
void err_ret(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));

void log_info(const char*, ...);
void log_msg(const char*, ...);
void log_open(const char*, int, int);
void log_quit(const char*, ...) __attribute__((noreturn));
void log_ret(const char*, ...);
void log_sys(const char*, ...) __attribute__((noreturn));
void log_exit(int, const char*, ...) __attribute__((noreturn));

#endif


