

/*
File: printTree.c

Description: functions for huffman tree printing

*/

#include "printTree.h"

/* MOVE TO PRINTTREE.C  */


int makePrintQ(node *n, int posL, int posR, int incR, printPar *state)
{
    treePart part = Leaf;
    int lSubTreeWidth;

    if(n==NULL){
        return incR;
    }

    printQAdd(state, part, n->chr, posL, posR);

    lSubTreeWidth = makePrintQ(n->c0, posL+1, posR, 0, state);

    posR += lSubTreeWidth;

    incR += makePrintQ(n->c1, posL, posR+1, 1 , state);

    incR += lSubTreeWidth;

    return incR;
}





printPar *printInit(void)
{
    printPar *state;

    myCalloc(state, 1, sizeof(printPar), MEMERR);

    strcpy(state->treeLookup[intNd].line0, ".-");
    strcpy(state->treeLookup[intNd].line1, "| ");

    strcpy(state->treeLookup[Leaf].line0,  "C ");
    strcpy(state->treeLookup[Leaf].line1,  "  ");

    strcpy(state->treeLookup[Pad].line0,   "--");
    strcpy(state->treeLookup[Pad].line1,   "  ");

    return state;
}

void printQAdd(printPar *state, treePart part, char c, int posL, int posR)
{
    printQ *qThis, *qCurr, *qPrev;

    myCalloc(qThis, 1, sizeof(printQ), MEMERR);

    qThis->posL = posL;
    qThis->posR = posR;
    qThis->part = part;
    qThis->c = c;

    qPrev = NULL;
    qCurr = state->Q;
    /* add to correct place in print queue*/
    while(qCurr!=NULL && qThis->posL >= qCurr->posL){
        qPrev = qCurr;
        qCurr = qCurr->next;
    }

    if(qPrev==NULL){
        qThis->next = qCurr;
        state->Q = qThis;
    } else {
        qThis->next = qCurr;
        qPrev->next = qThis;
    }
}


void printQueue(printPar *state)
{
    printQ *Q;

    Q = state->Q;

    while(Q!=NULL){
        printf("part %d, char %c, loc %d,%d\n",
                Q->part, Q->c, Q->posL, Q->posR);
        Q = Q->next;
    }
}



/*
print width * 2  + 1 spaces above

for inner node queue [.-] on {line} and [| ] on {line+1}
for leaf node queue  [C ] on {line} and [  ] on {line+1}
for pad queue width* [--] on {line} and [  ] on {line+1}

.-----.-------.-G
|     |       |
.-.-D .-.-.-A F
| |   | | |
H E   C B 0




.-----.-------.-G
|     |       |
.-.-D .-.-.-A F
| |   | | |
H C   E B _



.-----.-------.-g
|     |       |
.-.-d .-.-.-a f
| |   | | |
h c   e b


.---.-------.-g
|   |       |
.-.-


*/
