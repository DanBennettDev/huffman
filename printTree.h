
/*
File: printTree.h

Description: header for huffman tree printing functions

*/

#include "huffman.h"

#define TPRINTPART 2
#define TREEPARTS 4
#define ROWSPERLINE 2

typedef enum TreePart {
    intNd, Leaf, Conn, Space
} treePart;

enum LinePart {
    NODE, LEFTARM
};



typedef struct TreePrint {
    char line0[TPRINTPART + 1];
    char line1[TPRINTPART + 1];
} tPrint;


typedef struct printQueue {
    treePart part;
    char c;
    int posL;
    int posR;
    struct printQueue *next;
}   printQ;

typedef struct printParams {
    printQ * Q;
    tPrint treeLookup[TREEPARTS];
} printPar;



int makePrintQ(node *n, int posL, int posR, int incR, printPar *state);
printPar *printInit(void);
void printQAdd(printPar *state, treePart part, char c, int posL, int posR);
void printQueue(printPar *state);
void printNode(printQ *Q, int linepart, printPar *state);
