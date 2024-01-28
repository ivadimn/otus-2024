#include "common.h"
#include "tables.h"


int main(int argc, char** argv) {

    encode_t encode;

    if (argc != 4) {
        err_msg("Ошибка:  %s\n",  "Не правильное количество аргументов.");
        print_usage();
        exit(1);
    }
    
    if (strcmp(argv[2], "cp1251") == 0)
        encode = CP_1251;
    else if (strcmp(argv[2], "koi-8r") == 0)
        encode = KOI_8R;
    else if (strcmp(argv[2], "iso-8859-5") == 0)
        encode = ISO_8859_5;
    else {
        err_msg("Ошибка:  %s\n",  "Не верно указана кодировка.");
        print_usage();
        exit(1);
    }    
    
    to_utf8(argv[1], argv[3], encode);

    return 0;
}