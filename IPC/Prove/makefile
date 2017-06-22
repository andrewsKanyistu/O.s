# Sources
SRCS= es2threads.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=es2

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -pthread -o $@ $<

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) -pthread -o  $(OBJS) $< 


clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean