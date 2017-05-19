# Sources
SRCS= myshell.c myshell_with_fork.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=myshell.x myshell_with_fork.x

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

myshell.x: myshell.o
	$(CC) -o $@ $^

myshell_with_fork.x: myshell_with_fork.o
	$(CC) -o $@ $^

myshell.o: myshell.c 
	$(CC) $(CFLAGS) -o $@ $< 

myshell_with_fork.o: myshell_with_fork.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
