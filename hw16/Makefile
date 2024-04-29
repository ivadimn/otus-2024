
CC=gcc
AR=ar

CFLAG=-Wall -Wextra -Wpedantic

all: log test 

log: log_obj 
	 $(AR) crs liblog.a log.o

log_obj: log.c
	$(CC) -c $(CFLAGSC) log.c -o log.o

test: main.c
	$(CC) $(CFLAG) main.c liblog.a -o test

clean:
	rm -rf *.o 