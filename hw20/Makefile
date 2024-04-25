
CC=gcc

CFLAGSD=-DDAEMON -Wall -Wextra -Wpedantic -std=c11 `pkg-config --cflags sqlite3`
CFLAGSC=-Wall -Wextra -Wpedantic -std=c11 `pkg-config --cflags sqlite3` `pkg-config --libs sqlite3`
LIB=`pkg-config --libs sqlite3`

daemon: dem 
config: conf

all: daemon config

dem: main.c log.c turn_daemon.c server.c params.c db.c
	$(CC) $(CFLAGSD) main.c log.c turn_daemon.c server.c params.c db.c -o dem $(LIB)

conf: maindb.c log.c params.c db.c
	$(CC) $(CFLAGSC) maindb.c log.c params.c db.c -o conf $(LIB)

clean:
	rm -rf dem conf