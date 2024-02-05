#include "common.h"
#include "dict.h"
#include <fcntl.h>
#include <ctype.h>


void file_handle(char* filename) {

    int fd;
    long fsize, reading_size;
    alpha *buffer;
    alpha word[128];
    size_t idx = 0;
    long count = 1;
    long *value, nc=0;
    dict_t *dict;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
        err_sys("Ошибка открытия файла: %s", filename);

    fsize = get_file_size(fd);
    if (fsize == -1) {
        err_sys("Ошибка при определении размера файла: %s", filename);
    }

    if (fsize < BUF_SIZE) {
        reading_size = fsize * sizeof(alpha);
        buffer = (alpha*) alloc(reading_size);
    }
    else {
        reading_size = BUF_SIZE * sizeof(uint8_t);
        buffer = (alpha*) alloc(reading_size);
    }

    dict = create_dict(256, 0.72f, 2.0f, LONG);

    while ((reading_size = read(fd, buffer, reading_size)) > 0) {
        for (int i = 0; i < reading_size; i++) {
            if ((buffer[i] >> 7) & 0x01) {
                word[idx++] = buffer[i];
                continue;
            }
            if (isalpha(buffer[i]) || buffer[i] == '\'') {
                word[idx++] = buffer[i];
            }
            else {
                if (idx > 0)                 {
                    word[idx] = '\0';
                    value = (long*) get(dict, word);

                    if (value == NULL) {
                        put(&dict, word, &count);
                    }
                    else {
                       nc = *value += 1;
                       put(&dict, word, &nc);
                    }
                    idx = 0;
                }
            }
        }
    }
    printf("Количество вхождений слов в файл %s\n", filename);
    print_dict(dict);
    destroy_dict(dict);
    free(buffer);
    close(fd);
    return;
}

int main(int argc, char** argv) {

    if (argc < 2)  {
        err_msg("Не достаточно аргументов!");
        EXIT_FAILURE;
    }
    file_handle(argv[1]);
    return EXIT_SUCCESS;

}
