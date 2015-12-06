

/*
File: printTree.c

Description: functions for huffman tree printing

*/

#include "printTree.h"

/* MOVE TO PRINTTREE.C  */

int makePrintQbak(node *c0, unsigned width, unsigned line, printPar *state)
{
    treePart part = Leaf;
    unsigned i=0;


    if(c0->c1 != NULL){
        width += makePrintQ(c0->c1, 1, line, state);
    }
    if(c0->c0 != NULL){
        width += makePrintQ(c0->c0, 0, line + ROWSPERLINE, state);
    }

    if(c0->c1 !=NULL || c0->c1){
        part = iNode;
    }

    /* queue element */
    printQAdd(state, part, c0->chr, line);

    /* queue padding */
    while(i< width){
        printQAdd(state, Pad, 0, line);
        i++;
    }
    return width;
}



int makePrintQbak2(node *c0, unsigned width, unsigned line, printPar *state)
{
    treePart part = Leaf;
    unsigned i=0;


    if(c0->c1 !=NULL || c0->c1){
        part = iNode;
    }
    /* mark pos of this node */
    printf("line %d, part %d, char %c\n", line, part, c0->chr);
    printQAdd(state, part, c0->chr, line);


    /* do left branch & return width */
    if(c0->c0 != NULL){
        width += makePrintQ(c0->c0, 0, line + ROWSPERLINE, state);
        part = iNode;
    }

    /* queue padding */
    while(i< width){
        printQAdd(state, Pad, 0, line);
        i++;
    }

    /* do right branch */
    if(c0->c1 != NULL){
        width += makePrintQ(c0->c1, 1, line, state);
    }

    return width;
}



int makePrintQ(node *n, loc Printloc, int branchWdth, printPar *state)
{
    treePart part = Leaf;

    /* mark this node */
    if(n->c0 !=NULL || n->c1){
        part = iNode;
    }


}





printPar *printInit(void)
{
    printPar *state;

    myCalloc(state, 1, sizeof(printPar), MEMERR);

    strcpy(state->treeLookup[iNode].line0, ".-");
    strcpy(state->treeLookup[iNode].line1, "| ");

    strcpy(state->treeLookup[Leaf].line0,  "C ");
    strcpy(state->treeLookup[Leaf].line1,  "  ");

    strcpy(state->treeLookup[Pad].line0,   "--");
    strcpy(state->treeLookup[Pad].line1,   "  ");

    return state;
}

void printQAdd(printPar *state, treePart part, char c, int line)
{
    printQ *qThis, *qCurr, *qPrev;

    myCalloc(qThis, 1, sizeof(printQ), MEMERR);

    qThis->line = line;
    qThis->print = state->treeLookup[part];

    if(part==Leaf){
        qThis->print.line0[0] = c;
    }

    qPrev = NULL;
    qCurr = state->Q;
    /* add to front of correct line in queue*/
    while(qCurr!=NULL && qThis->line >= qCurr->line){
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
    unsigned i = 0, end = 0;

    while(end==0){
        end = 1;
        Q = state->Q;
        while(Q!=NULL){
            if(i == Q->line){
                end = 0;
                printf("%s", Q->print.line0);
            }
            if(i == Q->line+1){
                end = 0;
                printf("%s", Q->print.line1);
            }
            Q = Q->next;
        }
        i++;
        printf("\n");
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


*/
