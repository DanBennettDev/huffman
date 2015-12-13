
/*
File: huffsdl.h

Description: header for huffman tree printing functions

*/
#include <SDL2/SDL.h>
#include <math.h>
#include "huffvis.h"
#include <time.h>

#define TPRINTPART 2
#define WIN_W 1500u
#define WIN_H 1000u
#define PAD_W 10u
#define PAD_H 100u
#define SDL_LOOP_DELAY 20


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
#define SIBLING_SPACE 1
#define CIRCRAD 8
#define CIRCLEPOINTS 100.0



/* Font stuff (from neillsdl2.h*/
typedef unsigned short fntrow;
#define FNTWIDTH (sizeof(fntrow)*8)
#define FNTHEIGHT 18
#define FNTCHARS 96
#define FNT1STCHAR 32
#define FONTFILE "m7fixed.fnt"

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


typedef struct Cartesian {
    unsigned x;
    unsigned y;
}   cart;


typedef struct Contour {
    node *xmin;
    node *xmax;
    int y;
    struct Contour *next;
} contour;


void initRand(void);

/* Knuth functions */
void knuth_getCoords(node *tree, int y);
void knuth_drawTree(SDL_Simplewin *sw, node *tree,
                        fntrow fnt[FNTCHARS][FNTHEIGHT]);


/* Tilford-Reingold functions */
contour *rt_1_trackContour(node *this, contour *l, contour *r);
void rt_1_addContour(contour *top, node *xmin, node *xmax);
double rt_1_pushApart(contour *l, contour *r);


/* drawing functions */
cart getTreeCoord (node *this);
void SDL_myInit(SDL_Simplewin *sw);
void drawCirc(SDL_Simplewin *sw, cart centre, int radius);
void setRandColour(SDL_Simplewin *sw);


/*taken wholesale from neillsdl2.h */
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy);
void Neill_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy);
void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r);
