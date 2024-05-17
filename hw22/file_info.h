#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>

#define LINE_SIZE 16384
#define MAX_URL 4096

typedef enum {HOST, ID, USER, DATE, REQUEST, 
            STATUS, BYTES, REFERER, AGENT, END, COUNT} comb_t;


void init_mtx();
void release_mtx();
void remove_trees();
void* handle_file(void* arg);
size_t get_urls(unsigned char*** hosts, size_t** bytes);
void remove_urls(unsigned char*** hosts, size_t** bytes);

size_t get_refers(unsigned char*** refs, size_t** count);
void remove_refers(unsigned char*** refs, size_t** count);


#endif