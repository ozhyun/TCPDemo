#
# Makefile
#
#
CFLAGS = -g
LDFLAGS = -pthread

all: server client

server: server.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

client: client.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm -f server client *.o
