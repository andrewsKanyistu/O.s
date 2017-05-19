# Sources
SRCS= mylib.c myrm.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=myrm.x

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

mycat.o: myrm.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
