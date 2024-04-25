#include "log.h"
#include "server.h"
#include "turn_daemon.h"
#include "params.h"
#include "db.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>

static char *app_name = NULL;
int log_to_stderr;

static struct option options[] = {
		{"daemon", no_argument, 0, 'd'},
		{"console", no_argument, 0, 'c'},
		{"help", no_argument, 0, 'h'},
		{NULL, 0, 0, 0}
};

/*
 *  печатает help по использованию приложения
 */
void print_help(void)
{
	printf("\n Использование: %s [ОПЦИИ]\n\n", app_name);
	printf("  Options:\n");
	printf("   -h --help                 Печатает эту справку\n");
	printf("   -d --daemon               Запукск в режиме демона\n");
	printf("   -c --console              Запуск в интерактивном режиме\n");
	printf("\n");
}

int main(int argc, char** argv) {

	param_t param = {.name = "filename"};

    int value, option_index = 0;
    int is_daemon = 0;
    app_name = argv[0];
    log_to_stderr = 1;

    if (argc < 2) {
        print_help();
		exit(1);
	}

    while ((value = getopt_long(argc, argv, "hdc", options, &option_index)) != -1) {
		switch (value) {
			case 'c':
				is_daemon = 0;
                log_info("Интерактивный режим \n");
				break;
			case 'd':
				is_daemon = 1;
                log_info("Режим демона\n");
				break;
			case 'h':
				print_help();
				return EXIT_SUCCESS;
			case '?':
				print_help();
				return EXIT_FAILURE;
			default:
				break;
		}
	}
	
	open_db();
	if (query_value(&param) == -1) {
		close_db();
		log_quit("Не удалось получить значение параметра.");
	}
	close_db();

    if (is_daemon) {
        log_to_stderr = 0;
        turn_daemon(app_name);    
    }
	
	
    run_server(app_name, param.value);

    return EXIT_SUCCESS;

}