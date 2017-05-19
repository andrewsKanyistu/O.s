# Sources
SRCS= mylib.c myls.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=myls.x

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

$(EXECUTABLE): mylib.o myls.c 
	@echo Building $@
	@ $(LD) -o $@ $^

mylib.o: mylib.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<


.PHONY: all clean
