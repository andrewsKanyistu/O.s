# sources
SRCS= mysort.c mysort_corretto.c
OBJS=$(SRCS:.c=.o)
EXECUTABLE=mysort.x mysort_corretto.x

# config
CC=gcc
CFLAGS= -c 
LD=gcc

# target

all : mysort.x mysort_corretto.x
	@echo buiLDing $(EXECUTABLE)

mysort.x: mysort.o
	$(LD) -o $@ $^

mysort_corretto.x: mysort_corretto.o
	$(LD) -o $@ $^

mysort.o: mysort.c 
	$(CC) $(CFLAGS) -o $@ $< 

mysort_corretto.o: mysort_corretto.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.phony: all clean
