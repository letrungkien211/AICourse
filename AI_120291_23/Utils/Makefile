CC = g++
RM = rm -f
COPY = cp
CFLAGS	= -Wall -g -O2

BINDIR = ../Bin

INCLUDES  = 	`pkg-config opencv --cflags` \
		-I/usr/include/eigen3 \
		-I../Utils



LIBS = `pkg-config opencv --libs`

TARGET	= image2matrix
OBJS = data.o ../Utils/utils.o

all:	$(TARGET)


$(TARGET): % : %.o $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	$(COPY) $@ $(BINDIR)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<


clean:
	$(RM) $(TARGET) $(OBJS)  *.o *~ *#*


