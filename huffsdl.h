
/*
File: huffsdl.h

Description: header for huffman tree printing functions

*/
#include <SDL2/SDL.h>
#include <math.h>
#include "huffvis.h"

#define TPRINTPART 2
#define WIN_W 1000
#define WIN_H 1000
#define PAD_W 10
#define PAD_H 10
#define DRAW_W WIN_W - PAD_W
#define DRAW_H WIN_H - PAD_H
#define DRAWDELAY 20


/*colours */
#define COLOURS 256
#define COL_WIN_BG   60u, 30u, 30u
#define COL_WHITE    127u, 127u, 127u

#define TRANS   80u
#define OPAQUE  127u

/* Font stuff (from neillsdl2.h*/
typedef unsigned short fntrow;
#define FNTWIDTH (sizeof(fntrow)*8)
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32


#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif



typedef enum TreePart {
    intNd, Leaf, Conn, Space
} treePart;

enum LinePart {
    NODE, LEFTARM
};

/* simplewin struct from neillsdl2*/
typedef struct SDL_Simplewin {
    SDL_bool finished;
    SDL_Window *win;
    SDL_Renderer *renderer;
} SDL_Simplewin;


typedef struct printQueue {
    treePart part;
    char c;
    int posL;
    int posR;
    struct printQueue *next;
}   printQ;

typedef struct Cartesian {
    int x;
    int y;
}   cart;


int drawTree(SDL_Simplewin *sw, node *n, unsigned huffcode, int depth,
        printQ *Q);
cart getDrawPos(unsigned huffcode, unsigned depth, unsigned maxdepth);
void drawEdge(SDL_Simplewin *sw, unsigned huffcode, unsigned depth,
        unsigned maxdepth);
unsigned cheap_pow(unsigned base, unsigned exp);

void SDL_myInit(SDL_Simplewin *sw);
void Neill_SDL_Events(SDL_Simplewin *sw);
