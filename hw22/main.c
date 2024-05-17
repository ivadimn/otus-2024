#include "log.h"
#include "utils.h"
#include "file_info.h"
#include <time.h>
#include <pthread.h>

/*
 *  печатает help по использованию приложения
 */
void print_help(const char* app_name)
{
	printf("\n Использование: %s <кол-во потоков>  <каталог с лог файлами> \n", app_name);
}

int main(int argc, char** argv) {

	pthread_t *thread;
    size_t thindex = 0;
    size_t thcount = 0;
    size_t file_index = 0;
	size_t file_count = 0;
	char** flist = NULL;
	unsigned char** urls = NULL;
	size_t *bytes = NULL;
	unsigned char** refs = NULL;
	size_t *ref_count = NULL;
	size_t num_urls = 0, num_refers = 0;
	int result;
	time_t begin, end;

    if (argc < 3) {
        print_help(argv[0]);
		exit(1);
	}
	   
	file_count = get_count_files(argv[2]);
	if (file_count == 0)
		crit("Каталог: %s не содержит файлов\n", argv[2]);

	thcount = strtoul(argv[1], NULL, 10);
	if (thcount == 0)
		thcount = file_count;
	
    thread = (pthread_t*) alloc(sizeof(pthread_t) * thcount);
	printf("Будут обработаны следующие файлы:\n");
	flist = init_file_list(file_count);
	file_list(argv[2], flist);
	for (size_t i = 0; i < file_count; i++) {
		printf("%s\n", flist[i]);
	}
	printf("...\n");

	init_mtx();
	begin = time(NULL);
	while(1) {

        for (thindex = 0; thindex < thcount; thindex++) {
            if (file_index >= file_count)
                break;
            result = pthread_create(&thread[thindex], NULL, handle_file, flist[file_index++]);
            if (result)  {
                crit("Поток не может быть создан, ошибка: %d\n", result);
            }
        }
        for (size_t j = 0; j < thindex; j++)  {
            result = pthread_join(thread[j], NULL);
            if (result)  {
                crit("Поток не может быть присоединён, ошибка: %d\n", result);
            }
        }
        if (file_index >= file_count)
            break;
    }
	
	release_mtx();	
	end = time(NULL);
	printf("Файлы обработаны за время: %lf.\n", difftime(end, begin));	
	printf("\n");

	num_urls = get_urls(&urls, &bytes);
	printf("10 самых \"тяжёлых\" по трафику URL'ов\n");
	if (urls && bytes) 	{
		for (size_t i = num_urls - 1; i >= num_urls - 10; i--)	{
			printf("URL: %s - %ld байт.\n", urls[i], bytes[i]);
		}	
	}
	remove_urls(&urls, &bytes);
	printf("\n");

	num_refers = get_refers(&refs, &ref_count);
	printf("10 наиболее часто встречающихся Referer'ов\n");
	if (refs && ref_count) 	{
		for (size_t i = num_refers - 1; i >= num_refers - 10; i--)	{
			printf("Referer: %s - %ld\n", refs[i], ref_count[i]);
		}	
	}
	
	remove_refers(&refs, &ref_count);

	free_file_list(flist, file_count);
	remove_trees();
		
	return EXIT_SUCCESS;

}