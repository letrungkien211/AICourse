CC = gcc
CFLAGS = -g -O2

OBJS =	grid.o

all:	grid

grid: $(OBJS)
	$(CC) -o grid $(OBJS)

grid: grid.c


.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f core grid *.o
