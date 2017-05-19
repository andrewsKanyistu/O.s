SRCS= mysleep.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=mysleep.x

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)


clean:
	@echo Cleaning
	@rm -f *.o *~
	@rm -f $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo Building $@
	@ $(LD) -o $@ $^

mysleep.o: mysleep.c
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
