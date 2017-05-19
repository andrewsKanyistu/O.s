# Sources
SRCS= sig_between_brother.c sig_to_brother.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all : sig_between_brothers.x sig_to_brother.x
	@echo Building $(EXECUTABLE)

sig_to_brother.x: sig_to_brother.o
	$(CC) -o $@ $^

sig_between_brother.x: sig_between_brother.o
	$(CC) -o $@ $^

sig_to_brother.o: sig_to_brother.c 
	$(CC) $(CFLAGS) -o $@ $< 

sig_between_brother.o: sig_between_brother.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
