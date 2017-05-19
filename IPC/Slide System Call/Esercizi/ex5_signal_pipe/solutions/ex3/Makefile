# sources
SRCS= pipe_exec.c pipe_father_to_child.c pread.c 
OBJS=$(SRCS:.c=.o)
EXECUTABLE=pipe_exec.x pipe_father_to_child.x pread.x 


# config
CC=gcc
CFLAGS= -c 
LD=gcc

# target

all: pipe_exec.x pipe_father_to_child.x pread.x 
	@echo building $(EXECUTABLE)

pipe_exec.x: pipe_exec.o
	$(CC) -o $@ $^

pipe_father_to_child.x: pipe_father_to_child.o
	$(CC) -o $@ $^

pread.x: pread.o
	$(CC) -o $@ $^

pread.o: pread.c
	$(CC) $(CFLAGS) -o $@ $< 

pipe_father_to_child.o: pipe_father_to_child.c
	$(CC) $(CFLAGS) -o $@ $< 

pipe_exec.o: pipe_exec.c
	$(CC) $(CFLAGS) -o $@ $< 

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)


.phony: all clean
