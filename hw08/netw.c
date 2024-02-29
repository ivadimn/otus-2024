#include "common.h"
#include "netw.h"

size_t  write_data_cb(void *data, size_t size, size_t nmemb, void *clientp) {
    size_t realsize = size * nmemb;
    struct memory_block *mem = (struct memory_block *)clientp;
 
    char *ptr = realloc(mem->buffer, mem->length + realsize + 1);
    if(!ptr) {
        err_sys("Ошибка при распределении памяти...");
    }
 
    mem->buffer = ptr;
    memcpy(&(mem->buffer[mem->length]), data, realsize);
    mem->length += realsize;
    mem->buffer[mem->length] = 0;
 
    return realsize;
}

char* get_data(char* url) {
    CURL *handle = NULL;
    CURLcode res;
 
    struct memory_block data;
 
    data.buffer = (char*)malloc(1);  
    data.length = 0;    
 
    curl_global_init(CURL_GLOBAL_ALL);
     
    handle = curl_easy_init();
    if (handle == NULL) {
        curl_global_cleanup();
        err_quit("Ошибка инициализации бибилиотеки curl.");
    }

    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data_cb);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&data);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
    res = curl_easy_perform(handle);
 
    if(res != CURLE_OK) {
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        err_quit("Ошибка при получении данных: %s\n", curl_easy_strerror(res));
    }
    
    curl_easy_cleanup(handle);
    curl_global_cleanup();

    return data.buffer; 
}