#ifndef _DB_H_
#define _DB_H_

#include "params.h"
#include <sqlite3.h>

#define SQL_LEN 4096

void open_db(void);
void close_db(void);
int get_count(void);
void str_param(const char* data, char* param);

int select_all(param_t * param, int limit);
int select_one(param_t * param);

int insert(param_t * param);
int update(param_t * param);
int remove(const char* pname);

#endif