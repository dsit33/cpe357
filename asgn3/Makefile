TARGET1  = hencode
CC       = gcc
CCFLAGS  = -g -std=c89 -ansi -pedantic -Wall -Werror
LDFLAGS  = -lm
SOURCES1 = hencode.c huffmanTree.c mylib.c priorityQueue.c
INCLUDES = $(wildcard *.h)
OBJECTS1 = $(SOURCES1:.c=.o) 

all:$(TARGET1)

$(TARGET1):$(OBJECTS1)
	$(CC) -o $(TARGET1) $(LDFLAGS) $(OBJECTS1)

$(OBJECTS1):$(SOURCES1) $(INCLUDES)
	$(CC) -c $(CCFLAGS) $(SOURCES1)

clean:
	rm -f $(TARGET1) $(OBJECTS1)
