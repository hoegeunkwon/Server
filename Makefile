CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm -lpthread -lmysqlclient

all: ShieldServer

ShieldServer: shieldsql.o server.o shield_main.o
	$(CC) -o $@ $^ $(LDFLAGS)

shield_main.o: ./src/shield_main.c
	$(CC) $(CFLAGS) -c $<

server.o: ./src/server.c
	$(CC) $(CFLAGS) -c $<

shieldsql.o: ./src/shieldsql.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o ShieldServer
