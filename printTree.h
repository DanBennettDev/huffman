
/*
File: printTree.h

Description: header for huffman tree printing functions

*/

#include "huffvis.h"

#define TPRINTPART 2

#define INTNd0 ".-"
#define INTNd1 "| "

#define CONN   "--"
#define SPACE  "  "


typedef enum TreePart {
    intNd, Leaf, Conn, Space
} treePart;

enum LinePart {
    NODE, LEFTARM
};

typedef struct printQueue {
    treePart part;
    char c;
    int posL;
    int posR;
    struct printQueue *next;
}   printQ;

int makePrintQ(node *n, int posL, int posR, int incR, printQ **Q);
void printQAdd(printQ **Q, treePart part, char c, int posL, int posR);
void printQueue(printQ *Q);
void printNode(printQ *Q, int linepart);
void fillSpace(printQ *Q, int linepart);

