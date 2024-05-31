#ifndef _DB_H_
#define _DB_H_


#define SQL_LEN 4096
#define ERR_MSG_LEN 256
#define NAME_LEN 64
#define INFO_LEN 32
#define IP4_LEN 16


#define INVALID_PARAM -1

typedef enum { SQLITE, POSTGRESQL } engine_t;

typedef struct {
    char dbms[INFO_LEN];
    char host[NAME_LEN];
    char hostaddr[IP4_LEN];
    char dbname[INFO_LEN];
    char user[INFO_LEN];
    char password[INFO_LEN];
} conninfo_t;

void open_db(conninfo_t* conninfo);
void close_db(void);
char* get_err_msg();
int is_valid_param(const char* table, const char* column);

int avg(const char* table, const char* column, double* result);
int min(const char* table, const char* column, double* result);
int max(const char* table, const char* column, double* result);
int sum(const char* table, const char* column, double* result);
int disp(const char* table, const char* column, double* result);

#endif