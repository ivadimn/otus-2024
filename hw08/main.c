#include "common.h"
#include "json_func.h"
#include "netw.h"
#include <stdio.h>
#include <curl/curl.h>

#define MAX_LEN 128

char url[MAX_LEN];

int main(int argc, char** argv)
{
    json_object *root = NULL;
    char *data = NULL;

    if (argc != 2) {
        err_msg("Ошибка:  %s\n", "Не правильное количество аргументов.");
        exit(EXIT_FAILURE);
    }

    snprintf(url, MAX_LEN, "https://wttr.in/%s?format=j1", argv[1]);
    
    data = get_data(url);
    root = json_tokener_parse(data);     
    if (root == NULL) {
      err_quit("Не удалось получить JSON-объект: %s\n", data);
    }
    
    get_info(root);
    json_object_put(root);
    free(data);
    return 0;
}