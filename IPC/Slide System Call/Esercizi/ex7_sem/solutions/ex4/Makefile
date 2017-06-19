# Sources
SRCS= client.c server.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=client.x server.x

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

client.x: client.c
	$(CC) -o $@ $<

server.x: server.c
	$(CC) -o $@ $<

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
