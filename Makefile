CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm -lpthread -lmysqlclient

all: server

server: shieldsql.o
	$(CC) -o $@ $^ $(LDFLAGS)

shieldsql.o: ./src/shieldsql.c ./include/shieldsql.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o shieldServer
