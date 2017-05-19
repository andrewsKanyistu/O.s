# Sources
SRCS= mylib.c back_write.c backe_write_to_file.c
EXECUTABLE=back_write.x back_write_to_file.x 

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

back_write.x: mylib.o back_write.c
	@echo Building $@
	@$(CC) -o $@ $^

back_write_to_file.x: mylib.o back_write_to_file.c
	@echo Building $@
	@ $(CC) -o $@ $^

mylib.o: mylib.c mylib.h
	@echo $@
	@ $(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
