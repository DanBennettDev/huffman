
EX =
CFLAGS = `pkg-config --cflags cunit` -pedantic -Wall -Wextra -Wfloat-equal -std=c90 -O3
SOURCES = $(TARGET).c printTree.c
LIBS = `pkg-config --libs cunit`

INCS = $(EX).h
TARGET = $(EX)
TESTT = $(EX)_test
EX = huffman


CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS) .FORCE
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

$(TESTT): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TESTT) $(CFLAGS) $(LIBS)


printTree.o:


.FORCE:

clean:
	rm -f $(TARGET) $(TESTT)

run: $(TARGET)
	$(TARGET)

runtest: $(TESTT)
	$(TESTT)
