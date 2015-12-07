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
        return 0;
    }
    printQAdd(state, part, n->chr, posL, posR);

    lSubTreeWidth = makePrintQ(n->c0, posL+1, posR, 0, state);

    posR += lSubTreeWidth;
    incR += makePrintQ(n->c1, posL, posR+1, 1 , state);

    return incR + lSubTreeWidth;
}



printPar *printInit(void)
{
    printPar *state;

    myCalloc(state, 1, sizeof(printPar), MEMERR);

    strcpy(state->treeLookup[intNd].line0, ".-");
    strcpy(state->treeLookup[intNd].line1, "| ");

    strcpy(state->treeLookup[Leaf].line0,  "C ");
    strcpy(state->treeLookup[Leaf].line1,  "  ");

    strcpy(state->treeLookup[Conn].line0,   "--");
    strcpy(state->treeLookup[Conn].line1,   "  ");

    strcpy(state->treeLookup[Space].line0,   "  ");
    strcpy(state->treeLookup[Space].line1,   "  ");


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
    /* build in freeing*/
    printQ *Q;
    int currLine = 0, linepart = NODE;

    Q = state->Q;

    while(Q!=NULL){
        if(Q->posL > currLine){
            printf("\n");

            if(linepart == NODE){
                linepart = LEFTARM;
                Q = state->Q;
            } else {
                linepart = NODE;
                currLine = Q->posL;
            }
        }

        printNode(Q, linepart, state);

        Q = Q->next;
    }
}


void printNode(printQ *Q, int linepart, printPar *state)
{


    if(Q->part == intNd){
        if(linepart==Node){
            printf("%s", state->treeLookup[intNd].line0);

            if(Q->next->posR > Q->posR+1){
                fillSpace(Q, linepart, state);
            }




        } else { /* linepart - left arms */
            printf("%s", state->tPrint.Line1);
        }




    } else if (Q->part == Leaf){





    }

}

void fillSpace(printQ *Q, int linepart, printPar *state)
{
    int i = 0, diff;
    diff = (Q->next->posR - Q->posR) - 1;

    if(linepart==NODE){
        while(diff>0){
            printf("%s", state->treeLookup[Conn].line0);
            diff--;
        }
    }

    if(linepart==LEFTARM){
        while(diff>0){

        }
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
