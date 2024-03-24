#ifndef _FILE_INFO_H_
#define _FILE_INFO_H_

#include <stddef.h>
#include <time.h>

#define MAX_PATH 4096
#define MAX_NAME 256
#define MAX_DIR_PATH (MAX_PATH - MAX_NAME)

typedef enum {TYPE_NONE, TYPE_DIR, TYPE_FILE} file_t;

typedef struct file_info_t {
    char name[MAX_NAME];
    char full_name[MAX_PATH];
    size_t size;
    file_t type;
    time_t date;
    struct file_info_t *flist;
} file_info_t;

void file_info(file_info_t* finfo);
void get_file_list(file_info_t* dir);
file_info_t* create_dir( char *dir);
file_info_t* get_current_dir(void);
void delete_dir(file_info_t *dir);
void get_parent_dir_name(char* parent, char *dir);

#endif