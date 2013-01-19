CC = g++
RM = rm -f
LIBS = -lm
CFLAGS = -Wall -g

TARGET = cryptarithmetic nqueen
REPORT = AI_120291_06

all: $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) -c $<
$(TARGET): % : %.o
	$(CC) -o $@ $^ $(LIBS)

$(REPORT): % : %.tex
	latex $<
	pdflatex $<

clean:
	$(RM) $(TARGET) *#* *~ *.o
	ls | grep $(REPORT) | grep -v .tex | grep -v .pdf | xargs $(RM)

tar:
	tar -czvf $(addsuffix .tar.gz, $(REPORT)) *.cpp *.hpp Makefile