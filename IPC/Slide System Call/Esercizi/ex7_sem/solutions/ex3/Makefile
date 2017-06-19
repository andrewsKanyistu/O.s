# Sources
SRCS= prod_cons_completo.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=prod_cons_completo.x

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) -o $@ $<

$(OBJS): $(SRCS) 
	$(CC) $(CFLAGS) -o $(OBJS) $< 


clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
