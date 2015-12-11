/*
File: huffsdl.c

Description: functions for huffman tree drawing

Recursive implementation of Reingold-Tilford tree layout
see: Tidier Drawings of Trees
     Reingold & Tilford
     IEE Transactions on Software Engineering Vol SE-7 No2 March 1981

todo:
    Reingold Tilson tree drawing

    Tidy up:
        clear out old code
        strip out Neill's function and link to his files instead
    have mouseover on vertexes so they report huffcode

    exit button
    more efficient 1px circle method.
*/

#include "huffsdl.h"

int main(int argc, char *argv[])
{
    node *chars, *q=NULL, *root;
    SDL_Simplewin sw;
    fntrow font[FNTCHARS][FNTHEIGHT];



    if(argc!=2){
        fprintf(stderr, "incorrect usage, try eg"
                "./huffman textfile.txt");
        exit(1);
    }

    chars = getCharCounts(argv[1]);

    q = orderInputQ(chars, q);

    root = buildTree(q);

    /************* VISUALISATION ****************/
    /*set up screen*/
    SDL_myInit(&sw);
    Neill_SDL_ReadFont(font, FONTFILE);

/*  OLD DRAWING METHOD
    maxdepth = getDepth(root, 0);
    drawTree(&sw, root, 0, 0, maxdepth, font);
*/




    /* update window once */
    SDL_Delay(SDL_LOOP_DELAY);
    SDL_RenderPresent(sw.renderer);
    SDL_UpdateWindowSurface(sw.win);

    do { /* check for input
            TODO: replace with mouseover check and
            clickable button to exit  */

        Neill_SDL_Events(&sw);
    }
    while (!sw.finished);

    /************* VISUALISATION END ************/



    /* Clear up graphics subsystems */
    SDL_DestroyWindow(sw.win);
    atexit(SDL_Quit);

    /*clear up other memory */
    freeTree(root);
    free(chars);


    return 0;
}



/*  Reingold-Tilford tree layout implementation */

/* pass 1 :

*/
threads rt_draw_1(node *this)
{
    /* TODO handle 1 child case */
    threads threadL, threadR;
    double centre;

    if(this==NULL){
        threadL.l = threadL.r = NULL;
        return threadL;
    }

    threadL = rt_draw_1(this->c0);
    threadR = rt_draw_1(this->c1);

    /* assign raw x value & mod for children*/
    this->x = (this->parent->c0 == this) ?
                0 : this->parent->c0->x + SIBLING_SPACE;

    if(this->c0!=NULL && this->c1!=NULL){
    /* centre above children */
        centre = this->c1->x - this->c0->x;

        if(this->parent->c0 == this){
            this->x = centre;
        } else {
            this->off = this->x - centre;
        }
    }



    /* task 3 of R-T pass1 - add "thread" for contour tracking */
    if(this->c1 != NULL && this->c0 != NULL && this->c0->y != this->c1->y){
        rt_1_make_thread(this, threadL, threadR);
    }


    /* task 2 of R-T pass1 - track "thread" candidates */
    return rt_1_track_threads(this, threadL, threadR);
}


threads rt_1_track_threads(node *this, threads thL, threads thR)
{
    threads thRet;

    /* handle leaf/ non-full nodes*/
    if(thL.l==NULL) {
        thL.l = this;
    }
    if(thL.r==NULL) {
        thL.r = this;
    }
    if(thR.l==NULL) {
        thR.l = this;
    }
    if(thR.r==NULL) {
        thR.r = this;
    }

    /* pick leftmost node on the lowest level */
    if( thL.l->y == thR.l->y ){
        thRet.l = thL.l->x <= thR.l->x ? thL.l : thR.l;
    } else {
        thRet.l = thL.l->y >= thR.l->y ? thL.l : thR.l;
    }

    /* pick rightmost node on the lowest level */
    if(thL.r->y == thR.r->y ){
        thRet.r = thL.r->x >= thR.r->x ? thL.r : thR.r;
    } else {
        thRet.r = thL.r->y >= thR.r->y ? thL.r : thR.r;
    }

    return thRet;
}


void rt_1_make_thread(node *this, threads thL, threads thR)
{
    if(this->c0->y > this->c1->y){
        thR.r->thread = thL.r;
    } else {
        thL.l->thread = thR.l;
    }
}





/* less efficient for unfilled circle, but I wanted a
    1px circle outline. Quick and dirty. */
void drawCirc(SDL_Simplewin *sw, cart centre, int radius)
{
    double angle;
    int points = CIRCLEPOINTS, x, y, xprev=-1, yprev=-1;

    while(points-- > 0){
        angle = ((double)points / CIRCLEPOINTS) * 2 * M_PI;
        x = centre.x + radius * cos(angle);
        y = centre.y + radius * sin(angle);
        if(x!=xprev || y!=yprev){
            SDL_RenderDrawPoint(sw->renderer, x, y);
            yprev=y, xprev=x;
        }
    }
}


/*

first, simpleminded effort - deals badly  with dense lower layers

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


    if(depth){
        this = getDrawPos(huffcode, depth, maxd);
        parent = getDrawPos(huffcode>>1, depth-1, maxd);

        SDL_SetRenderDrawColor(sw->renderer, COL_WHITE, OPAQUE);
        SDL_RenderDrawLine(sw->renderer, parent.x, parent.y,
                                this.x, this.y);

        if(n->chr){
            if(n->bit==0){
                this.x -= FNTWIDTH;
            }

            Neill_SDL_DrawChar(sw, fnt, n->chr, this.x, this.y);
        } else {
            SDL_SetRenderDrawColor(sw->renderer, COL_WIN_BG, OPAQUE);
            Neill_SDL_RenderFillCircle(sw->renderer, this.x, this.y, CIRCRAD);
            SDL_SetRenderDrawColor(sw->renderer, COL_WHITE, OPAQUE);
            drawCirc(sw, this, CIRCRAD);
        }

    }
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
*/


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





/* the rest taken unchanged from neillsdl2 */

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


/* Filled Circle centred at (cx,cy) of radius r, see :
   http://content.gpwiki.org/index.php/SDL:Tutorials:Drawing_and_Filling_Circles */
void Neill_SDL_RenderFillCircle(SDL_Renderer *rend, int cx, int cy, int r)
{

   double dy;
   for (dy = 1; dy <= r; dy += 1.0) {
        double dx = floor(sqrt((2.0 * r * dy) - (dy * dy)));
        SDL_RenderDrawLine(rend, cx-dx, cy+r-dy, cx+dx, cy+r-dy);
        SDL_RenderDrawLine(rend, cx-dx, cy-r+dy, cx+dx, cy-r+dy);
   }

}

