#include "common.h"
#include "zip.h"

void print_info(uint16_t count, cfh_t **cfh_arr) {
    for (uint16_t i = 0; i < count; i++)     {
        print_file_info(i + 1, cfh_arr[i]);
    }
}

int main(int argc, char** argv) {
    
    int iz = 0;
    ecodr_t* ecodr = create_ecodr();
    cfh_t **cfh_arr;
    char ans = 'y';

    if (argc < 2) {
        err_msg("Недостаточно аргументов!");
        return 1;
    }

    iz = zip_preview(argv[1], ecodr);
    if (iz < 0) {
        printf("Файл %s не содержит архива.\n", argv[1]);
    }
    else if (iz == 0)
    {
        printf("Файл %s содержит пустой архив.\n", argv[1]);
    }
    else {
        printf("Файл %s содержит архив, содержащий %d файлов.\n", argv[1], ecodr->ecodr.cfh_count);
        print_ecodr_info(ecodr);
        printf("Выводить содержимое архива? (y/n) ");
        scanf("%c", &ans);
        if(ans == 'y' || ans == 'Y') {
            cfh_arr = zip_contains(argv[1], ecodr);
            if (cfh_arr != NULL)  {
                print_info(ecodr->ecodr.cfh_count, cfh_arr);
                delete_cfh_array(ecodr->ecodr.cfh_count, cfh_arr);
            }
        }
    }
    delete_ecodr(ecodr);
    return 0;
}
