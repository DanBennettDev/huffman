
/*
File: printTree.h

Description: header for huffman tree printing functions

*/

#include "huffman.h"

#define TPRINTPART 2
#define TREEPARTS 3
#define ROWSPERLINE 2

typedef enum TreePart {
    iNode, Leaf, Pad
} treePart;


typedef struct TreePrint {
    char line0[TPRINTPART + 1];
    char line1[TPRINTPART + 1];
} tPrint;

typedef struct printQueue {
    tPrint print;
    unsigned line;
    struct printQueue *next;
}   printQ;

typedef struct printParams {
    printQ * Q;
    tPrint treeLookup[TREEPARTS];
} printPar;



int makePrintQ(node *c0, unsigned width, unsigned line, printPar *state);
printPar *printInit(void);
void printQAdd(printPar *state, treePart part, char c, int line);
void printQueue(printPar *state);
