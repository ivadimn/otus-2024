#ifndef _JSON_FUNC_H
#define _JSON_FUNC_H

#include <json-c/json.h>


typedef struct  w_info_t {
    char* key;
    char* value;
    struct w_info_t *next;
    char* format;
} w_info_t;

void get_info();


#endif