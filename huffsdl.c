/*
File: huffsdl.c

Description: functions for huffman tree drawing

todo:
    write letters
    tidy SDL struct, huffcode, depth, maxdepth into substruct
    have mouseover on vertexes so they report huffcode
    exit button

*/

#include "huffsdl.h"

int main(int argc, char *argv[])
{
    node *chars, *q=NULL, *root;
    SDL_Simplewin sw;
    unsigned maxdepth;
    fntrow font[FNTCHARS][FNTHEIGHT];



    if(argc!=2){
        fprintf(stderr, "incorrect usage, try eg"
                "./huffman textfile.txt");
        exit(1);
    }

    chars = getCharCounts(argv[1]);

    q = orderInputQ(chars, q);

    root = buildTree(q);

    /* VISUALISATION */
    /*set up screen*/
    SDL_myInit(&sw);
    Neill_SDL_ReadFont(font, FONTFILE);


    maxdepth = getDepth(root, 0);
    drawTree(&sw, root, 0, 0, maxdepth, font);

    /* update window once */
    SDL_Delay(DRAWDELAY);
    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    do { /* check for input
            TODO: replace with mouseover check and
            clickable button to exit  */

        Neill_SDL_Events(&sw);
    }
    while (!sw.finished);

    /* Clear up graphics subsystems */
    SDL_DestroyWindow(sw.win);
    atexit(SDL_Quit);

    /*clear up other memory */
    freeTree(root);
    free(chars);


    return 0;
}


unsigned getDepth(node *n, unsigned d)
{
    unsigned d0, d1;
    if(n==NULL){
        return d-1;
    }

    d0 = getDepth(n->c0 ,d+1);
    d1 = getDepth(n->c1 ,d+1);

    return d0 > d1 ? d0: d1;
}


void drawTree(SDL_Simplewin *sw, node *n, unsigned huffcode, unsigned depth,
                unsigned maxd, fntrow fnt[FNTCHARS][FNTHEIGHT])
{
    cart this, parent;
    if(n==NULL){
        return;
    }


    huffcode = (huffcode<<1) + n->bit;

    drawTree(sw, n->c0, huffcode, depth+1, maxd, fnt);
    drawTree(sw, n->c1, huffcode, depth+1, maxd, fnt);

    SDL_SetRenderDrawColor(sw->renderer, COL_WHITE, TRANS);

    if(depth){
        this = getDrawPos(huffcode, depth, maxd);
        parent = getDrawPos(huffcode>>1, depth-1, maxd);

        SDL_RenderDrawLine(sw->renderer, parent.x, parent.y,
                                this.x, this.y);

        if(n->chr){
            if(n->bit==0){
                this.x -= FNTWIDTH;
            }

            Neill_SDL_DrawChar(sw, fnt, n->chr, this.x, this.y);
        }
    }

}

cart getDrawPos(unsigned huffcode, unsigned depth, unsigned maxdepth)
{
    unsigned membrs;
    cart coord;
    membrs = cheap_pow(2, depth) + 1;
    coord.x = ((huffcode+1) * WIN_W / membrs);
    coord.y = (depth * (WIN_H - PAD_H) / maxdepth);

    return coord;
}


unsigned cheap_pow(unsigned base, unsigned exp)
{
    int ret = 1;
    while(exp){
        if(exp & 1){
            ret *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return ret;
}


/* 90% taken from neillsdl2
changed to:
    grab input
    set bg colour from #define
    allow alpha blending (for mousover huffcode reporting- if I get that far!)
*/

void SDL_myInit(SDL_Simplewin *sw)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
      fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    sw->finished = 0;

    sw->win= SDL_CreateWindow("SDL Window",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WIN_W, WIN_H,
                            /*SDL_WINDOW_INPUT_GRABBED*/
                            0);
    if(sw->win == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
    if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    /* Set screen bg colour */
    SDL_SetRenderDrawColor(sw->renderer, COL_WIN_BG, OPAQUE);
    SDL_RenderClear(sw->renderer);
    SDL_RenderPresent(sw->renderer);

    SDL_SetRenderDrawBlendMode(sw->renderer, SDL_BLENDMODE_BLEND);
}





/* taken wholesale from neillsdl2 */

void Neill_SDL_Events(SDL_Simplewin *sw)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             sw->finished = 1;
       }
    }
}


void Neill_SDL_ReadFont(fntrow fontdata[FNTCHARS][FNTHEIGHT], char *fname)
{

    FILE *fp = fopen(fname, "rb");
    size_t itms;
    if(!fp){
       fprintf(stderr, "Can't open Font file %s\n", fname);
       exit(1);
   }
   itms = fread(fontdata, sizeof(fntrow), FNTCHARS*FNTHEIGHT, fp);
   if(itms != FNTCHARS*FNTHEIGHT){
       fprintf(stderr, "Can't read all Font file %s (%d) \n", fname, (int)itms);
       exit(1);
   }
   fclose(fp);

}


void Neill_SDL_DrawChar(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], unsigned char chr, int ox, int oy)
{

   unsigned x, y;
   for(y = 0; y < FNTHEIGHT; y++){
      for(x = 0; x < FNTWIDTH; x++){
         if(fontdata[chr-FNT1STCHAR][y] >> (FNTWIDTH - 1 - x) & 1){
            /*printf("*");*/
            /* White Ink */
            Neill_SDL_SetDrawColour(sw, 255, 255, 255);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
         else{
            /*printf(".");*/
            /* Black Ink */
            Neill_SDL_SetDrawColour(sw, 0, 0, 0);
            SDL_RenderDrawPoint(sw->renderer, x + ox, y+oy);
         }
      }
   }

}

void Neill_SDL_DrawString(SDL_Simplewin *sw, fntrow fontdata[FNTCHARS][FNTHEIGHT], char *str, int ox, int oy)
{

   int i=0;
   unsigned char chr;
   do{
      chr = str[i++];
      Neill_SDL_DrawChar(sw, fontdata, chr, ox+i*FNTWIDTH, oy);
   }while(str[i]);
}


void Neill_SDL_SetDrawColour(SDL_Simplewin *sw, Uint8 r, Uint8 g, Uint8 b)
{

   SDL_SetRenderDrawColor(sw->renderer, r, g, b, SDL_ALPHA_OPAQUE);

}
