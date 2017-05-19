# Sources
SRCS= mylib.c back_write_to_file_dup.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=back_write_to_file_dup.x

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

$(EXECUTABLE): mylib.o back_write_to_file_dup.c 
	@echo Building $@
	@ $(LD) -o $@ $^

mylib.o: mylib.c mylib.h
	@echo Building $@
	@ $(CC) $(CFLAGS) -o $@ $<


.PHONY: all clean
