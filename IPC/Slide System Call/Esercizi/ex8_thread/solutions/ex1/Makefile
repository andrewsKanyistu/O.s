# Sources
SRCS= matrix_serial.c matrix_threads_completo.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE= matrix_threads_completo.x matrix_serial.x

# Config
CC=gcc
CFLAGS= -c
LD=gcc
LDLIBS= -lpthread


# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

matrix_serial.x: matrix_serial.o
	$(CC) -o $@ $^

matrix_threads_completo.x: matrix_threads_completo.o
	$(CC) -o $@ $^ $(LDLIBS)

$matrix_threads_completo.o: matrix_threads_completo.c 
	$(CC) $(CFLAGS) -o $@ $< 

$matrix_serial.o: matrix_serial.c 
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
