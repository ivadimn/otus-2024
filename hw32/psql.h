#ifndef _PSQL_H_
#define _PSQL_H_

#include <libpq-fe.h>

#define QUERY_LEN 256

void open_psql(const char* host,
               const char* dbname,
               const char* user,
               const char* pwd);

void close_psql();
int is_valid_psql_param(const char* table, const char* column, char* err);

int select_psql_stat(const char* query, double* value, char* err);

#endif