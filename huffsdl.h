
/*
File: huffsdl.h

Description: header for huffman tree printing functions

*/
#include <SDL2/SDL.h>
#include <math.h>
#include "huffvis.h"

#define TPRINTPART 2
#define WIN_W 1500u
#define WIN_H 1000u
#define PAD_W 10u
#define PAD_H 30u
#define DRAWDELAY 20


/*colours */
#define COLOURS 256
#define COL_WIN_BG   0u, 0u, 0u
#define COL_WHITE    127u, 127u, 127u

#define TRANS   127u
#define OPAQUE  127u

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


unsigned getDepth(node *n, unsigned d);
void drawTree(SDL_Simplewin *sw, node *n, unsigned huffcode, unsigned depth,
                unsigned maxd, fntrow fnt[FNTCHARS][FNTHEIGHT]);
cart getDrawPos(unsigned huffcode, unsigned depth, unsigned maxdepth);

unsigned cheap_pow(unsigned base, unsigned exp);

void SDL_myInit(SDL_Simplewin *sw);

/*taken wholesale from neillsdl2.h */
void Neill_SDL_Events(SDL_Simplewin *sw);
void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy);
void Neill_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy);
void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname);
void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b);
