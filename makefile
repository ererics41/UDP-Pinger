CC = gcc
ARGS = -Wall

all: Ping_Client

PingClient: PingClient.c
	$(CC) $(ARGS) -o Ping_Client PingClient.c

clean:
	rm -f *.o Ping_Client *~