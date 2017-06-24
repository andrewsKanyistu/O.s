# Sources
SRCS= dotprod_serial.c dotprod_mutex_completo.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE= dotprod_serial.x dotprod_mutex_completo.x

# Config
CC=gcc
CFLAGS= -c
LD=gcc
LDLIBS= -lpthread

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

dotprod_serial.x: dotprod_serial.o
	$(CC) -o $@ $<

dotprod_mutex_completo.x: dotprod_mutex_completo.o
	$(CC) -o $@ $^ $(LDLIBS)

$dotprod_mutex_completo.o: dotprod_mutex_completo.c 
	$(CC) $(CFLAGS) -o $@ $< 

$dotprod_serial.o: dotprod_serial.c 
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
