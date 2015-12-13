
CFLAGS = -pedantic -Wall -Wextra -Wfloat-equal -std=c90 -O3
LIBS = `pkg-config --libs sdl2` -lm


EX1 = huffman
INC1 = $(EX1).h
SRC1 = $(EX1).c


EX2 = huffvis
INC2 = $(EX2).h printTree.h
SRC2 = $(EX2).c printTree.c


EX3 = huffsdl
INC3 = $(EX3).h $(EX2).h neillsdl2.h
SRC3 = $(EX3).c $(EX2).c neillsdl2.c


CC = gcc


all: clean $(EX1) $(EX2) $(EX3)

$(EX1): $(SRC1) $(INC2) .FORCE
	$(CC) $(SRC1) -o $(EX1) $(CFLAGS) $(LIBS)

$(EX2): $(SRC2) $(INC2) .FORCE
	$(CC) $(SRC2) -o $(EX2) $(CFLAGS) $(LIBS)

$(EX3): $(SRC3) $(INC3) .FORCE
	$(CC) $(SRC3) -o $(EX3) $(CFLAGS) $(LIBS) -DHUFFSDL_EX=1

.FORCE:

clean:
	rm -f $(EX1) $(EX2) $(EX3)

run: $(TARGET)
	$(TARGET)
