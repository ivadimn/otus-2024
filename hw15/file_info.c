#include "common.h"
#include "file_info.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

static file_info_t* curr_dir = NULL;

static size_t _find_last(char* str, char ch) {
    size_t index = -1;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == ch)
            index = i;    
    }
    return index;    
}

void get_parent_dir_name(char* parent, char *dir) {
    size_t pos = _find_last(dir, '/');
    if (pos <= 0) {
        parent[0] = '/';
        parent[1] = '\0';
    }
    else {
        strncpy(parent, dir, pos);    
        parent[pos] = '\0';
    }
}

file_info_t* get_current_dir(void) {
    return curr_dir;
}

size_t get_dir_size(char *dir) {

    struct dirent *dp = NULL;
    DIR *dfd = NULL;
    size_t count = 0;

    if((dfd = opendir(dir))==NULL){
        err_cont("Ошибка открытия каталога (in get_dir_size): %s", dir);
        return 0;
    }

    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0)
            continue;
        count++;    
    }
    closedir(dfd); 
    return count;   
}

void get_file_info(file_info_t* finfo) {
    struct stat stat_buff;
    int result = stat(finfo->full_name, &stat_buff);
    if (result == -1) {
        err_cont("Ошибка при получении информации о файле: %s", finfo->full_name);
        finfo->type = TYPE_NONE;
        return;
    }
    
    if (S_ISDIR(stat_buff.st_mode)) {
        finfo->type = TYPE_DIR;
        finfo->size = get_dir_size(finfo->full_name);
    }
    else {
        finfo->type = TYPE_FILE;
        finfo->size = stat_buff.st_size;
    }
    finfo->flist = NULL;
    finfo->date = stat_buff.st_ctime;
    return;
}

void get_file_list(file_info_t *dir) {

    char name[MAX_PATH + MAX_NAME];
    struct dirent *dp;
    DIR *dfd;
    size_t index = 0;

    if((dfd = opendir(dir->full_name))==NULL) {
        err_cont("Ошибка открытия каталога (in get_file_list): %s", dir->full_name);
        return;
    }

    dir->flist = (file_info_t*) malloc(sizeof(file_info_t) * dir->size);
    if (dir->flist == NULL) {
        err_sys("Ошибка выделения памяти!");
    }
    
    while((dp=readdir(dfd)) != NULL){
        
        if(strcmp(dp->d_name,".") == 0 || 
            strcmp(dp->d_name,"..") == 0 )
            continue;
        if (strcmp(dir->full_name, "/") == 0)
            sprintf(name,"%s%s",dir->full_name, dp->d_name);    
        else
            sprintf(name,"%s/%s",dir->full_name, dp->d_name);
        strcpy(dir->flist[index].name, dp->d_name);
        strcpy(dir->flist[index].full_name, name);        
        get_file_info(&dir->flist[index]);
        index++;
    }
    closedir(dfd);
}

file_info_t* create_dir( char *dir) {

    curr_dir = (file_info_t*) malloc(sizeof(file_info_t));
    if (curr_dir == NULL) {
        err_sys("Ошибка выделения памяти!");
    }
    curr_dir->type = TYPE_DIR;
    strcpy(curr_dir->name, dir);
    strcpy(curr_dir->full_name, dir);
    curr_dir->flist = NULL;
    curr_dir->size = get_dir_size(dir);
    return curr_dir;
}


void delete_dir(file_info_t *dir) {
    static int level = 0;
    if (dir) {
        if (dir->flist) {
            for (size_t i = 0; i < dir->size; i++) {
                if (dir->flist[i].type == TYPE_DIR && dir->flist[i].size > 0) {
                    level++;
                    delete_dir(&dir->flist[i]);
                }
            }   
            free(dir->flist);
        }
    }
    if (level == 0) {
        free(dir);
    }
}

