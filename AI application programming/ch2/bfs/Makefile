CC = gcc

OBJS = bfs.o

all:	bfs

bfs: $(OBJS)
	$(CC) -o bfs $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f bfs *.o
