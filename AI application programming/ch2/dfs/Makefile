CC = gcc

OBJS = dfs.o

all:	dfs

bfs: $(OBJS)
	$(CC) -o dfs $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f dfs *.o
