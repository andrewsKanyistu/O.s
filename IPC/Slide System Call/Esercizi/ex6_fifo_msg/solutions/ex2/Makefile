# sources
SRCS= server.c client1.c client2.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=server.x client1.x client2.x

# config
CC=gcc
CFLAGS= -c 
LD=gcc

# target

all : server.x client1.x client2.x
	@echo building $(EXECUTABLE)

server.x: server.o
	$(CC) -o $@ $^

server.o: server.c 
	$(CC) $(CFLAGS) -o $@ $<

client1.x: client1.o
	$(CC) -o $@ $^

client1.o: client1.c
	$(CC) $(CFLAGS) -o $@ $< 

client2.x: client2.o
	$(CC) -o $@ $^

client2.o: client2.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.phony: all clean
