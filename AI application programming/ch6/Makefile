#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	main.o

all:	cls

cls: $(OBJS) clstypes.h
	$(CC) -o cls $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f cls *.o
