# sources
SRCS= myshell.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=myshell.x

# config
CC=gcc
CFLAGS= -c 
LD=gcc

# target

all : $(EXECUTABLE)
	@echo buiLDing $(EXECUTABLE)

myshell.x: myshell.o
	$(CC) -o $@ $^

myshell.o: myshell.c 
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.phony: all clean
