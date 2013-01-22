#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	main.o init.o fitness.o swarm.o

all:	swarm

swarm: $(OBJS) pso.h
	$(CC) -o swarm $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f swarm *.o particle??.txt
