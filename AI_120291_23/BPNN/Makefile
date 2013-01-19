CC = g++
RM = rm -f
MLDIR = $(HOME)/Dropbox/git/ML

CFLAGS	= -Wall -g -O2
INCLUDES=	`pkg-config opencv --cflags` \
		-I/usr/include/eigen3	\
		-I$(MLDIR)/Utils

LIBS = 		`pkg-config opencv --libs`


OBJS =  bpnn.o $(MLDIR)/Utils/utils.o
all:	$(OBJS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<


clean:
	$(RM)  $(TARGET) $(OBJS) *.o *~



