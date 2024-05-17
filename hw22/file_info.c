#include "log.h"
#include "file_info.h"
#include "utils.h"
#include "tree.h"
#include "sort.h"
#include <pthread.h>

#include <sys/mman.h>
#define URL_PC  3

const size_t buff_size = 8 * 1024 * 1024;
static node_t* tree_urls = NULL;
static node_t* tree_refers = NULL;
static unsigned char url[MAX_PART];


static void save_data(unsigned char** arr); 

static pthread_mutex_t mtx;

void remove_trees() {
    tree_delete(tree_urls);
    tree_delete(tree_refers);
}
void init_mtx() {
    pthread_mutex_init(&mtx, NULL);
}
void release_mtx() {
    pthread_mutex_destroy(&mtx);
}

void *handle_file(void* arg) {
    char* filename = (char*)arg;
    int fd;
    size_t read_count = 0;
    long index = 0;
    char *g_open = "\"["; 
    char *g_close = "\"]"; 
    unsigned char* line;
    unsigned char *fbuff = NULL;
    unsigned char** arr = NULL;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err("Ошибка открытия файла: %s", filename);
        pthread_exit(NULL);
    }

    line = (unsigned char*)alloc(sizeof(unsigned char) * LINE_SIZE);
    fbuff = (unsigned char*)alloc(sizeof(unsigned char) * buff_size);
    arr = (unsigned char**)alloc(sizeof(unsigned char*) * COUNT);
    for (size_t i = 0; i < COUNT; i++) {
        arr[i] = (unsigned char*)alloc(sizeof(unsigned char) * MAX_PART);
    }

    do {

        read_count = read(fd, fbuff, buff_size);
        for (size_t i = 0; i < read_count; i++) {
            if (fbuff[i] == '\n')  {
                line[index] = '\0';
                index = 0;
                split((char**)arr, (char*)line, ' ', g_open, g_close);
                save_data((unsigned char**)arr);
            }
            else {
                line[index++] = fbuff[i];
            }
        }
    } while (read_count == buff_size);

    for (size_t i = 0; i < COUNT; i++) {
        free(arr[i]);
    }
    free(arr);
    free(fbuff);    
    free(line);
    close(fd);
    printf("Файл: %s обработан\n", filename);
    pthread_exit(NULL);
}

static void save_data(unsigned char** arr)  {
    
    long bytes = strtol((char*)arr[BYTES], NULL, 10);
    long url_start = -1, url_end = -1, len = -1;
    
    unsigned char* request = arr[REQUEST];
    unsigned char* ref = arr[REFERER];

    pthread_mutex_lock(&mtx);
    if (bytes > 0) {
        url_start = str_in((char*)request, '/');
	    url_end = str_instr((char*)request, " HTTP");
        len = str_substr((char*) url, (char*)request, url_start, url_end);
        if(len > 0) {
            str_urldecode((char*)url);
            push(&tree_urls, url, bytes);
        }
        else {
            str_urldecode((char*)request);
            push(&tree_urls, request, bytes);
        }
    }

    if (strcmp((char*)ref, "-") != 0) {
        str_urldecode((char*)ref);
        push(&tree_refers, ref, 1);    
    }
    
    pthread_mutex_unlock(&mtx);
    
}

size_t get_urls(unsigned char*** urls, size_t** bytes) {
    size_t urls_count = tree_count_nodes(tree_urls);
    *urls = (unsigned char**) alloc(sizeof(unsigned char*) * urls_count);
    *bytes = (size_t*) alloc(sizeof(size_t) * urls_count);
    get_leafs(tree_urls, *urls, *bytes);
    sort_shell(*urls, *bytes, 0, urls_count - 1);
    return urls_count;
}

void remove_urls(unsigned char*** hosts, size_t** bytes) {
    free(*bytes);
    free(*hosts);
}

size_t get_refers(unsigned char*** refs, size_t** count) {
    size_t ref_count = tree_count_nodes(tree_refers);
    *refs = (unsigned char**) alloc(sizeof(unsigned char*) * ref_count);
    *count = (size_t*) alloc(sizeof(size_t) * ref_count);
    get_leafs(tree_refers, *refs, *count);
    sort_shell(*refs, *count, 0, ref_count - 1);
    return ref_count;
}

void remove_refers(unsigned char*** refs, size_t** count) {
    free(*count);
    free(*refs);
}