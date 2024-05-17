#ifndef _UTILS_H_
#define _UTILS_H_

#include <stddef.h>

#define MAX_PATH 1024
#define MAX_PART 8192

typedef enum state { ONE, TWO } state_t;

char** init_file_list(size_t count);
void free_file_list(char** flist, size_t count);

size_t get_file_size(int fd);
int is_dir(const char* file);

void file_list(char *dir, char** flist);
size_t get_count_files(char *dir);
void* alloc(size_t size);

void split(char** arr, char* str, char delim, char* g_open, char* g_close);
long str_in(char *str, char ch);

long str_instr(char *str, char* s);
long str_substr(char* dest, char *str, long start, long end);
void str_urldecode(char *str);

#endif