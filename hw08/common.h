#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define BUF_SIZE 4096

void err_msg(const char*, ...);
void err_cont(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));
void err_quit(const char*, ...) __attribute__((noreturn));

#endif


