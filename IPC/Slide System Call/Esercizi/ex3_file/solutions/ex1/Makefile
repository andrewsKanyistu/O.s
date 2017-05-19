# Sources
SRCS= mylib.c mycat.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=mycat.x

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


mylib.o: mylib.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<

mycat.o: mycat.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
