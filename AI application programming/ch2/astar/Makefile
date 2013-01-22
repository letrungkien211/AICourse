CC = gcc

OBJS = main.o list.o misc.o

all:	astar

astar: $(OBJS)
	$(CC) -o astar $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f astar *.o
