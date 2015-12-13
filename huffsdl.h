
/*
File: huffsdl.h

Description: header for huffman tree printing functions

usage: huffsdl /path/to/textfile.txt

expects ASCII / utf-8 text file.

*/
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
#include "neillsdl2.h"
#include "huffvis.h"

#define TPRINTPART 2
#define WIN_W 1000u
#define WIN_H 800u
#define PAD_W 10u
#define PAD_H FNTHEIGHT * 2
#define TITLE_POS PAD_H / 3
#define SDL_LOOP_DELAY 20


#define FONTFILE "m7fixed.fnt"

/*colours */
#define RGB_MMBRS 3
#define COLOURS 256
#define COL_WIN_BG   0u, 0u, 0u
#define COL_WHITE    255u, 255u, 255u
#define COL_TREE_GREEN 0u, 100u, 0u

/* lights*/
#define LIGHT_CHANGE_DELAY 300
#define LIGHT_COLS 4
#define COL_LIGHT_RED 200u, 0u, 0u
#define COL_LIGHT_BLUE 0u, 0u, 200u
#define COL_LIGHT_WHITE 255u, 255u, 255u
#define COL_LIGHT_GREEN 0u, 255u, 0u


#define TRANS   80u
#define OPAQUE  255u


/* layout */
#define LAYER_HEIGHT 2
#define KNUTH_SPACE 1
#define RT_SPACE 1.5
#define CIRCRAD 8
#define CIRCLEPOINTS 100.0


typedef enum TreePart {
    intNd, Leaf, Conn, Space
} treePart;

enum LinePart {
    NODE, LEFTARM
};


typedef struct Cartesian {
    unsigned x;
    unsigned y;
}   cart;


typedef struct Contour {
    double xmin;
    double xmax;
    struct Contour *next;
} contour;


void initRand(void);

/* Knuth functions */
void knuth_getCoords(node *tree, int y);


void drawTree(SDL_Simplewin *sw, node *tree,
                        fntrow fnt[FNTCHARS][FNTHEIGHT]);


/* drawing functions */
void doDisplay(node *tree);
cart getTreeCoord (node *this);
void SDL_myInit(SDL_Simplewin *sw);
void setRandColour(SDL_Simplewin *sw);



/* Tilford-Reingold layout functions (not used in this version yet) */
contour *rt_draw_1(node *this, int depth);
contour *rt_1_trackContour(node *this, contour *l, contour *r, double space);
contour *rt_1_addContour(contour *top, double xmin, double xmax);
double rt_1_pushApart(contour *l, contour *r);
double rt_1_1_move_onscreen(node *tree);
void rt_2_petrify(node *tree, double offset);
void freeContour(contour *cont);
