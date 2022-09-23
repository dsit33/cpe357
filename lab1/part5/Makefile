CC = gcc
CFLAGS = -Wall -ansi -pedantic
MAIN = example
OBJS = example.o fact.o
all : $(MAIN)

$(MAIN) : $(OBJS) fact.h
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

example.o : example.c fact.h
	$(CC) $(CFLAGS) -c example.c

fact.o : fact.c fact.h
	$(CC) $(CFLAGS) -c fact.c

clean :
	rm *.o $(MAIN) core
