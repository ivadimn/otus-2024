#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define BUF_SIZE 4096

void err_msg(const char*, ...);
void err_cont(const char*, ...);
void err_sys(const char*, ...) __attribute__((noreturn));
int64_t getFileSize(int);

#endif


