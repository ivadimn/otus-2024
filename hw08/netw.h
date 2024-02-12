#ifndef _NETW_H_
#define _NETW_H_

#include <stdint.h>
#include <curl/curl.h>

struct memory_block {
    char *buffer;
    size_t length;
};

size_t  write_data_cb(void *data, size_t size, size_t nmemb, void *clientp);
char* get_data(char* url);


#endif