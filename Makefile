
EX =
CFLAGS = -pedantic -Wall -Wextra -Wfloat-equal -std=c90 -O3
SOURCES = $(TARGET).c printTree.c
LIBS =

INCS = $(EX).h
TARGET = $(EX)
EX = huffvis


CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS) .FORCE
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)


.FORCE:

clean:
	rm -f $(TARGET)

run: $(TARGET)
	$(TARGET)
