/*
File: huffman.c

Description: calculates huffman codes for a document.

usage: huffman /path/to/textfile.txt

expects ASCII / utf-8 text file.

*/


#include <stdio.h>  /* FILE, printf, fprintf, scanf, */
#include <stdlib.h> /* exit, calloc, malloc, free */
#include <ctype.h> /* tolower */

#define MEMERR "memory allocation error at line %d", __LINE__

#define myCalloc(A, B, C, D)   \
            do{A = calloc(B, C); \
            if(A==NULL){ \
                fprintf(stderr, D); \
                exit(1);}\
            } while(0)

#define ASCIICHARS 127
#define BITSINBYTE 8

typedef struct Node {
                char chr;
                int weight;
                int bit;
                struct Node *parent;
                struct Node *c0;
                struct Node *c1;
                struct Node *qNext;
} node;


FILE * openFile(char *fName);
node *getCharCounts(char *filename);
node *makeNode(void);
node *orderInputQ(node *in, node *queue);
node *pairandRequeue(node *nd, node *q2Top, node *q2Tail, node *c0, node *c1);
node *dequeueNode(node *nd);
node *buildTree(node *q1Top);
void printEncoding(node *chars);
node *getNextItem(node **q1, node **q2);
void revStr(char *str, int len);
void freeTree(node *chars);



int main(int argc, char *argv[])
{
    node *chars, *q=NULL, *root;

    if(argc!=2){
        fprintf(stderr, "incorrect usage, try eg"
                "./huffman textfile.txt");
        exit(1);
    }

    chars = getCharCounts(argv[1]);

    q = orderInputQ(chars, q);

    root = buildTree(q);

    printEncoding(chars);

    freeTree(root);
    free(chars);

    return 0;
}



node *getCharCounts(char *filename)
{
    FILE *fp;
    int c;
    node *chars;

    myCalloc(chars, ASCIICHARS, sizeof(node), MEMERR);

    /*populate letter names*/
    for(c=0; c<ASCIICHARS; c++){
        chars[c].chr = c;
    }

    fp = openFile(filename);
    while((c=fgetc(fp)) != EOF) {
        if(c >=0 && c < ASCIICHARS){
            chars[c].weight++;
        }
    }
    fclose(fp);
    return chars;
}


FILE * openFile(char *fName)
{
    FILE *fp = fopen(fName, "r");
    if(!fp){
        fprintf(stderr,"error loading file,"
                    " check filepath\n");
        exit(1);
    }
    return fp;
}


node *makeNode(void)
{
    node *nd;
    myCalloc(nd, 1, sizeof(node), MEMERR);

    return nd;
}


node *orderInputQ(node *in, node *qTop)
{	/* put members of input array into ordered queue */

    int c;
    node *qCurr, *qPrev, *thisC;

    for(c=0; c<ASCIICHARS; c++){
    	/* re-init */
        thisC = &in[c];
        qCurr = qTop;
        qPrev = NULL;

        /* find pos */
        while(qCurr!=NULL && thisC->weight > qCurr->weight){
            qPrev = qCurr;
            qCurr = qCurr->qNext;
        }

        /* insert into q */
        if(qPrev==NULL){
            thisC->qNext = qCurr;
            qTop = thisC;
        } else {
            thisC->qNext = qCurr;
            qPrev->qNext = thisC;
        }
    }
    return qTop;
}


node *buildTree(node *q1Top)
{	/* using 2 queues. New par-child-child(...) grp goes to
     back of 2nd queue (avoids resorting) */

    node *child0, *child1, *parent, *q2Top=NULL, *q2Tail=NULL;

    while (q1Top->weight==0){
        q1Top = q1Top->qNext;
    }

    do{
        if((child0 = getNextItem(&q1Top, &q2Top))==NULL){
            fprintf(stderr, "error, no tree root node\n");
            exit(1);
        }
        if((child1 = getNextItem(&q1Top, &q2Top))==NULL){
            return child0; /* root node*/
        }

        parent = makeNode();
        q2Top = pairandRequeue(parent, q2Top, q2Tail, child0, child1);
        q2Tail = parent;

    } while(1);
}


node *getNextItem(node **q1, node **q2)
{
    node *temp;
    if(*q1!=NULL && *q2!=NULL){
    /* if exist items in both queues, pick lowest weight*/

        if((*q1)->weight <= (*q2)->weight) {
            temp = *q1;
            *q1 = dequeueNode(*q1);
            return temp;
        } else {
            temp = *q2;
            *q2 = dequeueNode(*q2);
            return temp;
        }

    } else if (*q1!=NULL) {
        temp = *q1;
        *q1 = dequeueNode(*q1);
        return temp;

    } else if (*q2!=NULL) {
        temp = *q2;
        *q2 = dequeueNode(*q2);
        return temp;
    }
    return NULL;
}


node *pairandRequeue(node *new, node *q2Top, node *q2Tail, node *c0, node *c1)
{
    /* pair children beneath parent */
    new->weight = c0->weight + c1->weight;
    new->c0=c0;
    new->c1=c1;

    c0->parent = new;
    c1->parent = new;

    c0->bit=0;
    c1->bit=1;

    /* add to 2nd queue */
    if(q2Top==NULL){
        /* base case - empty 2nd queue */
        return new;
    }
    q2Tail->qNext = new;
    return q2Top;
}


node *dequeueNode(node *nd)
{
    node *q;
    q = nd->qNext;
    nd->qNext = NULL;
    return q;
}


void printEncoding(node *chars)
{
    unsigned c, depth=0, bits=0;
    node *nP;
    char huffcode[ASCIICHARS] = ""; /* worst case longest huffman code */

    for(c=0; c<ASCIICHARS; c++){
    	/*alphabetical order */
        depth=0;
        nP=&chars[c];


        if(chars[c].weight){
            /* get reverse code */
            while(nP->parent!=NULL){
                huffcode[depth++]= '0' + nP->bit;
                nP=nP->parent;
            }
            huffcode[depth] = '\0';
            /* reverse to get correct code */
            revStr(huffcode, depth);

            if(!isprint(chars[c].chr)){
                printf("%03d : ", (int)chars[c].chr);
            } else {
                printf("\'%c\' : ", chars[c].chr);
            }
            bits += (depth * chars[c].weight);
            printf("%20s (%4d * %8d)\n", huffcode, depth, chars[c].weight);
        }
    }
    printf("%d bytes\n", (bits / BITSINBYTE) + (bits % BITSINBYTE ==0 ? 0:1));
}

void revStr(char *str, int len)
{
    char *l, *r, swap;
    l = &str[0];
    r = &str[len-1];

    while(l<r){
        swap = *l;
        *l = *r;
        *r = swap;
        l++, r--;
    }
}

void freeTree(node *nP)
{
    if(nP->c0 == NULL){
        return;
    }
    freeTree(nP->c0);
    freeTree(nP->c1);

    free(nP);
}
