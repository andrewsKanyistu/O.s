# Sources
SRCS= mylib.c create_hole.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=create_hole.x

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

$(EXECUTABLE): mylib.o create_hole.c 
	@echo Building $@
	@ $(LD) -o $@ $^

mylib.o: mylib.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<


.PHONY: all clean
