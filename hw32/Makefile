
CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic `pkg-config --cflags sqlite3 libpq` 
LIBS=`pkg-config --libs sqlite3 libpq`

all: parse

parse: main.c log.c db.c lite.c psql.c
	$(CC) $(CFLAGS) main.c log.c db.c lite.c psql.c -o stat $(LIBS)

clean:
	rm -rf stat