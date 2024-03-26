#include "common.h"
#include "crc32.h"


int main(int argc, char** argv) {

    uint32_t crc;

    if (argc < 2) {
        err_msg("Ошибка:  %s\n",  "Не правильное количество аргументов.");
        exit(1);
    }
    
    crc = file_crc(argv[1]);
    printf("Контрольная сумма файла %s (CRC32) = %x\n", argv[1], crc);

    return 0;
}