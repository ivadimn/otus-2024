#include "params.h"
#include "db.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int query_value(param_t* param) {
    return select_one(param);
}

#ifndef DAEMON
int get_line(char* line, int size) {
    char ch;
    int index = 0;
    while ((ch = getchar()) != '\n' && index < size - 1) {
        line[index++] = ch;
    }
    line[index] = '\0';    
    return index - 1;
}

void get_param_name(char *pname) {
    printf("Введите наименование параметра: ");
    get_line(pname, NAME_LEN);
}

void get_param_value(char *pvalue) {
    printf("Введите значение параметра: ");
    get_line(pvalue, VALUE_LEN);
}
void get_param_exvalue(char *pexvalue) {
    printf("Введите дополнительное значение параметра (необязательно): ");
    get_line(pexvalue, EX_VALUE_LEN);
}

void get_param_description(char *pdesc) {
    printf("Введите описание параметра (необязательно): ");
    get_line(pdesc, MAX_LEN);
}

void get_full_param(param_t *param) {
    get_param_name(param->name);
    get_param_value(param->value);
    get_param_exvalue(param->ex_value);
    get_param_description(param->description);    
}

void print_params_list(void) {
    param_t *params = NULL;
    int count = get_count();
    params = (param_t*) malloc(sizeof(param_t) * count);

    if (params == NULL) 
        err_sys("Ошибка распеределения памяти.");

    select_all(params, count);
    printf("%-20s%-32s%-20s%-32s\n", "Name", "Value", 
                                     "Ex. value ", "Description");
    for (size_t i = 0; i < 92; i++)
        putchar('-');
    printf("\n");    
    
    for (int i = 0; i < count; i++)  {
        printf("%-20s%-32s%-20s%-32s\n", params[i].name, params[i].value,
                                              params[i].ex_value, params[i].description);    
    }
}

int add_param() {

    param_t param = {.ex_value[0] = '\0', .description[0] = '\0'};
    int rc = 0;
    get_full_param(&param);
    rc = insert(&param);
    printf("Параметр добавлен.\n");
    return rc; 
}

int change_param() {
    param_t param = {.ex_value[0] = '\0', .description[0] = '\0'};
    int rc = 0;
    get_param_name(param.name);
    printf("Введите новые значения: %s\n", param.name);
    get_param_value(param.value);
    get_param_exvalue(param.ex_value);
    get_param_description(param.description); 

    rc = update(&param);
    printf("Параметр %s изменён.\n", param.name);
    return rc; 
}

int remove_param() {
    char pname[NAME_LEN];
    int rc = 0;
    get_param_name(pname);
    rc = remove(pname);
    printf("Параметр %s удалён.\n", pname);
    return rc;
}

#endif