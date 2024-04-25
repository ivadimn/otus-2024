#ifndef _PARAMS_H_
#define _PARAMS_H_

#define NAME_LEN 32
#define VALUE_LEN 64
#define EX_VALUE_LEN 64
#define MAX_LEN 256
#define SQL_LEN 4096

typedef struct {
    int id;
    char name[NAME_LEN];
    char value[VALUE_LEN];
    char ex_value[EX_VALUE_LEN];
    char description[MAX_LEN];
} param_t;

int query_value(param_t* param);

#ifndef DAEMOM
int get_line(char* line, int size);
void get_param_name(char *pname);
void get_param_value(char *pvalue);
void get_param_exvalue(char *pexvalue);
void get_param_description(char *pname);



void print_params_list(void);
int add_param();
int change_param();
int remove_param();
#endif

#endif