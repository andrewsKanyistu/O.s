# Sources
SRCS= cmp_lists.c cmp_lists_completo.c scrivi_file.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE= cmp_lists.x cmp_lists_completo.x scrivi_file.x

# Config
CC=gcc
CFLAGS= -c
LD=gcc
LDLIBS= -lpthread

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

cmp_lists.x: cmp_lists.o
	$(CC) -o $@ $<

cmp_lists_completo.x: cmp_lists_completo.o
	$(CC) -o $@ $< $(LDLIBS)

$cmp_lists_completo.o: cmp_lists_completo.c 
	$(CC) $(CFLAGS) -o $@ $< 

$cmp_lists.o: cmp_lists.c 
	$(CC) $(CFLAGS) -o $@ $< 

scrivi_file.x: scrivi_file.o
	$(CC) -o $@ $< $(LDLIBS)

$scrivi_file.o: scrivi_file.c 
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.PHONY: all clean
