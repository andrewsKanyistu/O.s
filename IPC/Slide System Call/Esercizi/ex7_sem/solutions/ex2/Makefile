# Sources
SRCS= processA.c processB.c processC.c
EXECUTABLE=processA.x processB.x processC.x

# Config
CC=gcc
CFLAGS= -c 
LD=gcc

# Target

all: $(EXECUTABLE)  
	@echo Building $(EXECUTABLE)

processA.x: processA.c
	$(CC) -o $@ $<

processB.x: processB.c
	$(CC) -o $@ $<

processC.x: processC.c
	$(CC) -o $@ $<

clean:
	rm -f $(EXECUTABLE)	

.PHONY: all clean
