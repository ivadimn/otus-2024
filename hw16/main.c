#include "log.h"

void func() {
    int fd;
    fd = open("unkhownfile", O_RDWR, FILE_MODE);
    if (fd == -1){
        err("Ошибка открытия файла");    
    }
    
}

int main(int argc, char** argv) {
    
    if(argc > 1) {
        log_open(argv[1]);    
    }
    debug("Отладочное сообщение %d.", 255);
    info("Информационное сообщение %d.", 255);
    warning("Предупреждение %d.", 255);
    func();
    log_close();
    return 0;
}