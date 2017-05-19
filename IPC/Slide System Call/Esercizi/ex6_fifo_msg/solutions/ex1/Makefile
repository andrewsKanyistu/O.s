# sources
SRCS= server.c client.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=server.x client.x

# config
CC=gcc
CFLAGS= -c 
LD=gcc

# target

all : server.x client.x
	@echo building $(EXECUTABLE)

server.x: server.o
	$(CC) -o $@ $^

client.x: client.o
	$(CC) -o $@ $^

server.o: server.c 
	$(CC) $(CFLAGS) -o $@ $< 

client.o: client.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.phony: all clean


