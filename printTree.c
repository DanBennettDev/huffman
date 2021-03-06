/*
File: printTree.c

Description: functions for huffman tree printing

*/

#include "printTree.h"

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
    /* fix prespacing on newline*/
    printQ *Qthis = Q, *Qprev = NULL;
    int currLine = 0, linepart = NODE, i;


    while(Qthis!=NULL){
        if(Qthis->posL != currLine){ /* line changes */
            printf("\n");

            if(linepart == NODE){
                linepart = LEFTARM;
                Qthis = Q;
            } else {
                linepart = NODE;
                currLine = Qthis->posL;
            }
            /*pad to newline start*/
            for(i=0; i<Qthis->posR; i++){
                printf("%s", SPACE);
            }
        }
        printNode(Qthis, linepart);
        Qprev = Qthis;
        Qthis = Qthis->next;
        /*on 2nd run free elements */
        if(linepart==LEFTARM){
            free(Qprev);
            Q = Qthis;
        }
    }
    printf("\n");

    /* tidy up final line*/
    Qthis = Q;
    while(Qthis!=NULL){
        Qprev = Qthis;
        Qthis = Qthis->next;
        free(Qprev);
    }
}


void printNode(printQ *Q, int linepart)
{
    if(Q->part == intNd){
        if(linepart==NODE){
            printf("%s", INTNd0);
            fillSpace(Q, linepart);

        } else { /* linepart - left arms */
            printf("%s", INTNd1);
            fillSpace(Q, linepart);
        }

    } else { /* tree part = leaf */
        if(linepart==NODE){
            printf("%c ", Q->c);
            fillSpace(Q, linepart);

        } else { /* linepart - left arms */
            printf("%s", SPACE);
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
            printf("%s", CONN);
            diff--;
        }
    } else {
        while(diff>0){
            printf("%s", SPACE);
            diff--;
        }

    }
}
