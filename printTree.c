/*
File: printTree.c

Description: functions for huffman tree printing




todo
    Get rid of state struct
*/



#include "printTree.h"



/* MOVE TO PRINTTREE.C  */


int makePrintQ(node *n, int posL, int posR, int incR, printQ **Q)
{
    treePart part;
    int lSubTreeWidth;

    if(n==NULL){
        return 0;
    }

    if(n->c0!=NULL || n->c1!=NULL){
        part = intNd;
    } else {
        part = Leaf;
    }

    printQAdd(Q, part, n->chr, posL, posR);

    lSubTreeWidth = makePrintQ(n->c0, posL+1, posR, 0, Q);

    posR += lSubTreeWidth;
    incR += makePrintQ(n->c1, posL, posR+1, 1 , Q);

    return incR + lSubTreeWidth;
}



void printQAdd(printQ **Q, treePart part, char c, int posL, int posR)
{
    printQ *qThis, *qCurr, *qPrev;

    myCalloc(qThis, 1, sizeof(printQ), MEMERR);

    qThis->posL = posL;
    qThis->posR = posR;
    qThis->part = part;
    qThis->c = c;

    qPrev = NULL;
    qCurr = *Q;
    /* add to correct place in print queue*/
    while(qCurr!=NULL && qThis->posL >= qCurr->posL){
        qPrev = qCurr;
        qCurr = qCurr->next;
    }

    if(qPrev==NULL){
        qThis->next = qCurr;
        *Q = qThis;
    } else {
        qThis->next = qCurr;
        qPrev->next = qThis;
    }
}


void printQueue(printQ *Q)
{
    /* build in freeing*/
    printQ *Qthis = Q, *Qprev = NULL;
    int currLine = 0, linepart = NODE;

    while(Qthis!=NULL){
        if(Qthis->posL > currLine){ /* line changes */
            printf("\n");

            if(linepart == NODE){
                linepart = LEFTARM;
                Qthis = Q;
            } else {
                linepart = NODE;
                currLine = Qthis->posL;
                free(Qprev);
                Q = Qthis;
            }
        }
        printNode(Qthis, linepart);
        Qprev = Qthis;
        Qthis = Qthis->next;
    }
    printf("finished\n");
}


void printNode(printQ *Q, int linepart)
{
    if(Q->part == intNd){
        if(linepart==NODE){
            PRINTPART(INTNd0);
            fillSpace(Q, linepart);

        } else { /* linepart - left arms */
            PRINTPART(INTNd1);
            fillSpace(Q, linepart);
        }

    } else { /* tree part = leaf */
        if(linepart==NODE){
            PRINTLEAF1(Q->c);
            fillSpace(Q, linepart);

        } else { /* linepart - left arms */
            PRINTPART(SPACE);
            fillSpace(Q, linepart);
        }
    }
}



void fillSpace(printQ *Q, int linepart)
{
    int diff;
    if(Q->next==NULL || Q->next->posR == Q->posR+1){
        return;
    }

    diff = (Q->next->posR - Q->posR) - 1;

    if(linepart==NODE && Q->part == intNd){
        while(diff>0){
            PRINTPART(CONN);
            diff--;
        }
    } else {
        while(diff>0){
            PRINTPART(SPACE);
            diff--;
        }

    }
}
