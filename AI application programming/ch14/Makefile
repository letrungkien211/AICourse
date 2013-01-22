#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	main.o

all:	nlp

nlp: $(OBJS) nlptypes.h
	$(CC) -o nlp $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f nlp *.o
