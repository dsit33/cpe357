CC = gcc
CFLAGS = -g -Wall -ansi -pedantic
MAIN = example
OBJS = example.o
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

example.o : example.c
	$(CC) $(CFLAGS) -c example.c

clean :
	rm *.o $(MAIN) core*
