#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

all:	backprop mlp-bp mlp-btch

backprop: backprop.o
	$(CC) -o backprop backprop.o -lm

mlp-bp: mlp-bp.o
	$(CC) -o mlp-bp mlp-bp.o -lm

mlp-btch: mlp-btch.o
	$(CC) -o mlp-btch mlp-btch.o -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f backprop mlp-bp mlp-btch *.o stats.txt
