/*
File: huffsdl.c

Description: functions for huffman tree drawing

Recursive implementation of Reingold-Tilford tree layout
see: Tidier Drawings of Trees
     Reingold & Tilford
     IEE Transactions on Software Engineering Vol SE-7 No2 March 1981

todo:
    Reingold Tilson tree drawing
        Record Y coord from bottom (use y in rt)
        part 1 of rT pass 1
        Test pass1
        pass 2 (etc.?)

    Change colours so it's an Xmas binary tree.


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
contour *rt_draw_1(node *this)
{
    /* TODO handle 1 child case */
    contour *contL, *contR, *cont;
    double space;

    if(this==NULL){
        stateL.lthread = stateL.rthread = NULL;
        return stateL;
    }

    contL = rt_draw_1(this->c0);
    contR = rt_draw_1(this->c1);

    space = rt_1_pushApart(contL, contR);
    /* handle space - do I need to shift right member
       how do I centre parent?
    */

    cont = rt_1_trackContour(this, contL, contR);
    return cont
}


double rt_1_pushApart(contour *l, contour *r)
{
    double push = 0;

    while(l!=NULL && r!=NULL){
        if(l->xmax > r->xmin + SIBLING_SPACE){
            push = (l->xmax - r->xmin) + SIBLING_SPACE;
        }

        l=l->next;
        r=r->next;
    }
    return push;
}



contour *rt_1_trackContour(node *this, contour *l, contour *r)
{
    /*  combines task 2 and task 3 of RT pass 1
        (for recursive seems to make more sense to do together
         and using linked list rather than following threads) */

    /*  keep linked list of rt structs
        first item = this level (insert now)
        each item reports level min and max
        in this function compare the items, 1 by 1 and get new linked list
        then insert {this, this} as contour for this level.
    */
    contour *ret, *temp;
    node xmin, xmax;
    double ymax, offl, offr;

    /*add this level*/
    rt_1_addContour(ret, this->y, this, this);

    while(l!=NULL || r!=NULL){
        /* continue to the bottom of tree even if depths are unequal*/
        r = r==NULL ? l : r;
        l = l==NULL ? r : l;

        ymax = l.y > r.y ? l.y : r.y;
        xmin = this->offset + (l.xmin.x < r.xmin.x ? l.xmin : r.xmin);
        xmax = this->offset + (l.xmax.x > r.xmax.x ? l.xmax : r.xmax);

        addContour(ret, ymax, xmin, xmax);
        temp = l;
        l = l->next;
        free(temp);
        temp = r;
        r = r->next;
        free(temp);
    }
    return ret;
}

void rt_1_addContour(node *top, double y, node *xmin, node *xmax)
{
    contour cont;

    myCalloc(cont, 1, sizeof(contour), MEMERR);
    cont->xmin = xmin;
    cont->xmax = xmax;
    cont->y = y;

    if(top==NULL){
        top->next = cont;
        return;
    }
    while(top->next!=NULL){
        top= top->next;
    }
    cont->next = cont;
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

