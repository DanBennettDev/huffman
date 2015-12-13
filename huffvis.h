
/*
File: huffman.h

Description: header for huffman encoding functions

usage: huffvis /path/to/textfile.txt

expects ASCII / utf-8 text file.

*/

#include <stdio.h>  /* FILE, printf, fprintf, scanf, */
#include <stdlib.h> /* exit, calloc, malloc, free */
#include <ctype.h> /* tolower */
#include <string.h> /*strcpy */

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
                double x;       /* for sdl vis */
                double y;       /* for sdl vis */
                double offset;     /* for sdl vis */
                struct Node *parent;
                struct Node *c0;
                struct Node *c1;
                struct Node *contour;    /* for sdl vis */
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
