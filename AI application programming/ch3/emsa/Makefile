#
# Makefile for emhttp
#

CC = gcc

OBJS =	emsa.o

#all:	emsa

emsa: $(OBJS)
#	$(CC) -g -o emsa $(OBJS) -lm
	$(CC) -g -o $@ $+ -lm

.c.o:
	$(CC) $(CFLAGS) -g -Wall -c $<

emsa.o:	common.h

clean:
	rm -f emsa *.o core stats.txt
