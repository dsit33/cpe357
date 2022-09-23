CC = gcc
CFLAGS = -Wall -ansi -pedantic -g
MAIN = layout
OBJS = layout.o
all : $(MAIN)

$(MAIN) : $(OBJS)
	$(CC) $(CFLAGS) -o $(MAIN) $(OBJS)

layout.o : layout.c
	$(CC) $(CFLAGS) -c layout.c

clean :
	rm *.o $(MAIN) core
