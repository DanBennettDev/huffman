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
    printQ *pQ = NULL;
    SDL_Simplewin sw;

    unsigned test;

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

    drawTree(&sw, root, 0, 0, pQ);

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

    for(test=0; test< 10; test++){
        printf("%d : %d : %d\n", test, test<<1, (test<<1) + 1);
    }


    return 0;
}


int drawTree(SDL_Simplewin *sw, node *n, unsigned huffcode, int depth,
        printQ *Q)
{
    int maxd, d;

    if(n==NULL){
        return 0;
    }

    printf("bit: %d\n", n->bit);
    huffcode = (huffcode<<1) + n->bit;

    d = drawTree(sw, n->c0, huffcode, depth+1, Q);
    maxd = depth > d ? depth : d;

    d = drawTree(sw, n->c1, huffcode, depth+1, Q);
    maxd = maxd > d ? maxd : d;

    SDL_SetRenderDrawColor(sw->renderer, COL_WHITE, TRANS);
    if(depth){
        drawEdge(sw, huffcode, depth, maxd);
    }
    return maxd;
}


cart getDrawPos(unsigned huffcode, unsigned depth, unsigned maxdepth)
{
    unsigned membrs;
    cart coord;

    membrs = cheap_pow(2, depth);
    printf("membrs: %d\n",membrs);
    coord.x = (int) (huffcode * DRAW_W / membrs);
    coord.y = (int)((maxdepth -depth) * DRAW_H / maxdepth);
    return coord;
}

void drawEdge(SDL_Simplewin *sw, unsigned huffcode, unsigned depth,
        unsigned maxdepth)
{
    cart parent, this;

    this = getDrawPos(huffcode, depth, maxdepth);
    parent =  getDrawPos(huffcode>>1, depth-1, maxdepth);

    printf(" huff %u, huff>> %u, depth %u, maxdepth %u ",
                huffcode, huffcode>>1, depth, maxdepth);
    printf(" %d,%d : %d,%d\n", parent.x, parent.y, this.x, this.y);

    SDL_RenderDrawLine(sw->renderer, parent.x, parent.y, this.x, this.y);

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
                            SDL_WINDOW_INPUT_GRABBED);
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


/* Temporary measure - taken wholesale from neillsdl2
    TODO: replace with clickable button to exit
*/

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
