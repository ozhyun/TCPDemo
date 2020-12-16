#
# Makefile
#
#
CFLAGS = -g
LDFLAGS = -pthread

all: server client

server: server.c msg.c
	gcc $(CFLAGS) $(LDFLAGS) -o $@ $^

client: client.c msg.c
	gcc $(CFLAGS) $(LDFLAGS) -lrt -o $@ $^

clean:
	rm -f server client *.o
