
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic 

all: parse

parse: main.c file_info.c log.c utils.c sort.c tree.c
	$(CC) $(CFLAGS) main.c file_info.c log.c utils.c sort.c tree.c -o pth

clean:
	rm -rf pth