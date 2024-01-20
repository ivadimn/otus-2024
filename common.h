#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096
#define BUF_SIZE 4096

void err_msg(const char*, ...);
void err_cont(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));
int readf(int, void*, unsigned);

#endif


