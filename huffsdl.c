/*
File: huffsdl.c

Description: functions for huffman tree drawing

usage: huffsdl /path/to/textfile.txt

expects ASCII / utf-8 text file.

Knuth visualisation will handle any binary tree
currently Reingold Tilford vis assumes tree is complete (eg huffman)


todo:
    Reingold Tilford tree layout

*/

#include "huffsdl.h"

int main(int argc, char *argv[])
{
    node *chars, *q=NULL, *root;
    SDL_Simplewin sw;
    fntrow font[FNTCHARS][FNTHEIGHT];
    int drawcounter=0, c;
    unsigned  mode;
    double offset;


    if(argc!=2){
        fprintf(stderr, "incorrect usage, try eg"
                "./huffman textfile.txt");
        exit(1);
    }

    printf("\nenter number for layout type:\n");
    printf("0. Knuth, 1. Reingold-Tilford (with parent centering bug): ");

    /* build huffman tree */
    chars = getCharCounts(argv[1]);
    q = orderInputQ(chars, q);
    root = buildTree(q);


    /************* VISUALISE TREE ****************/
    while(scanf("%u", &mode)!=1 || mode > 1){
        printf("invalid choice, try again\n");

        do{/*clear out user input*/
            c=getchar();
        } while(c!='\n');

    }
    if(mode == 0){
        knuth_getCoords(root, 0);
    } else {
        rt_draw_1(root, 0);
        offset = rt_1_1_move_onscreen(root);
        root->offset = offset;
        root->x += offset;
        rt_2_petrify(root,0);
    }



    /*set up screen*/
    SDL_myInit(&sw);
    Neill_SDL_ReadFont(font, FONTFILE);

    do { /* draw and animate tree */
        SDL_Delay(SDL_LOOP_DELAY);
        drawcounter += SDL_LOOP_DELAY;

        Neill_SDL_Events(&sw);

        if(drawcounter>= LIGHT_CHANGE_DELAY){
            drawcounter=0;
            drawTree(&sw, root, font);
            Neill_SDL_DrawString(&sw, font, "XMAS (BINARY) TREE",
                                        PAD_W, PAD_H- FNTHEIGHT);

            /* update window once */
            SDL_RenderPresent(sw.renderer);
            SDL_UpdateWindowSurface(sw.win);
        }
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



void initRand(void){
   time_t now;
   time(&now);
   srand((int)now);
}

void knuth_getCoords(node *tree, int y)
{
    static int x = 0;
    if(tree==NULL){
        return;
    }
    knuth_getCoords(tree->c0, y+LAYER_HEIGHT);
    tree->x = x;
    x+= KNUTH_SPACE;
    tree->y = y;
    knuth_getCoords(tree->c1, y+LAYER_HEIGHT);
}


void drawTree(SDL_Simplewin *sw, node *tree,
                        fntrow fnt[FNTCHARS][FNTHEIGHT])
{
    cart parent, this;

    if(tree==NULL){
        return;
    }
    drawTree(sw, tree->c0, fnt);
    drawTree(sw, tree->c1, fnt);

    if(tree->y > 0){
        this = getTreeCoord(tree);
        parent = getTreeCoord(tree->parent);


        SDL_SetRenderDrawColor(sw->renderer, COL_TREE_GREEN, OPAQUE);
        SDL_RenderDrawLine(sw->renderer, parent.x, parent.y,
                                this.x, this.y);

        if(tree->chr){
            Neill_SDL_DrawChar(sw, fnt, tree->chr,
                                 this.x-(FNTWIDTH/2), this.y);
        } else {
            setRandColour(sw);
            Neill_SDL_RenderFillCircle(sw->renderer, this.x, this.y, CIRCRAD);
            SDL_SetRenderDrawColor(sw->renderer, COL_WHITE, OPAQUE);
            /*drawCirc(sw, this, CIRCRAD);*/
        }

    }
}

cart getTreeCoord (node *this)
{
    cart coord;
    coord.x = (this->x * FNTHEIGHT) + PAD_W;
    coord.y = (this->y * FNTWIDTH) + PAD_H;

    return coord;
}

void setRandColour(SDL_Simplewin *sw)
{
    unsigned cols[LIGHT_COLS][RGB_MMBRS]
                = {{COL_LIGHT_RED}, {COL_LIGHT_BLUE}, {COL_LIGHT_WHITE},
                    {COL_LIGHT_GREEN}};
    int rnd;

    rnd = rand() % LIGHT_COLS;

    SDL_SetRenderDrawColor(sw->renderer,
                            cols[rnd][0],cols[rnd][1],cols[rnd][2],
                                OPAQUE);
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
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n",
                                                        SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    sw->renderer = SDL_CreateRenderer(sw->win, -1, 0);
    if(sw->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n",
                                                        SDL_GetError());
      SDL_Quit();
      exit(1);
    }

    /* Set screen bg colour */
    SDL_SetRenderDrawColor(sw->renderer, COL_WIN_BG, OPAQUE);
    SDL_RenderClear(sw->renderer);
    SDL_RenderPresent(sw->renderer);

    SDL_SetRenderDrawBlendMode(sw->renderer, SDL_BLENDMODE_BLEND);
}



/*  Reingold-Tilford tree layout implementation */
/* pass 1 :

*/
contour *rt_draw_1(node *this, int depth)
{
    /* TODO assumes full tree for the moment: handle 1 child case */
    contour *contL, *contR, *cont;
    double space, xtemp;

    if(this==NULL){
        return NULL;
    }

    contL = rt_draw_1(this->c0, depth+LAYER_HEIGHT);
    contR = rt_draw_1(this->c1, depth+LAYER_HEIGHT);

    this->y = depth;

    /* pos relative to children */
    xtemp=0;
    if(this->c0!=NULL && this->c1!=NULL){
        xtemp = (this->c1->x - this-> c0->x)/2;
    }


    if(this->bit==0){
        this->x = xtemp;
    } else if(this->parent!=NULL){
        this->x = this->parent->c0->x + RT_SPACE;
        this->offset = this->x - xtemp;
    }

    space = rt_1_pushApart(contL, contR);
    printf("%c\n", this->chr);

    this->x += space/2;
    if(this->c1!=NULL){
        this->c1->x += space;
        this->c1->offset += space;

    }

    cont = rt_1_trackContour(this, contL, contR, space);

    if(depth==0){
        freeContour(cont);
    }

    return cont;
}


void freeContour(contour *cont)
{
    contour *temp;
    while(cont!=NULL){
        temp = cont;
        cont = cont->next;
        free(temp);
    }
}


double rt_1_pushApart(contour *l, contour *r)
{
    double push = 0;

    while(l!=NULL && r!=NULL){
        if(l->xmax >= r->xmin + push){
            push = (l->xmax - r->xmin) + RT_SPACE;
        }

        printf("%f , %f | ", l->xmax, r->xmin);

        l=l->next;
        r=r->next;
    }

    printf("\npush: %f\n", push);

    return push;
}



contour *rt_1_trackContour(node *this, contour *l, contour *r, double space)
{
    /*  combines task 2 and task 3 of RT pass 1
        (for recursive seems to make more sense to do together
         and using linked list rather than following threads) */

    contour *ret=NULL, *temp;
    double xmin, xmax;

    /*add this level*/
    ret = rt_1_addContour(ret, this->x, this->x);



    while(l!=NULL || r!=NULL){
        /* continue to the bottom of tree even if depths are unequal*/
        r = r==NULL ? l : r;
        l = l==NULL ? r : l;

        xmin = l->xmin < r->xmin + space ? l->xmin : r->xmin + space;
        xmax = l->xmax > r->xmax + space ? l->xmax : r->xmax + space;

        /* apply spacing*/

        ret = rt_1_addContour(ret, xmin, xmax);

        temp = l;
        l = l->next;
        free(temp);
        temp = r;
        r = r->next;
        if(r!=l){
            free(temp);
        }
    }
    return ret;
}


contour *rt_1_addContour(contour *top, double xmin, double xmax)
{
    contour *cont, *this;

    myCalloc(cont, 1, sizeof(contour), MEMERR);
    cont->xmin = xmin;
    cont->xmax = xmax;

    /* add to contour list */
    if(top==NULL){

        return cont;
    }

    this=top;

    while(this->next!=NULL){
        this= this->next;
    }
    this->next = cont;

    return top;
}





double rt_1_1_move_onscreen(node *tree)
{
    double temp, min;
    if(tree==NULL){
        return 0;
    }

    temp = rt_1_1_move_onscreen(tree->c0);
    min = tree->x < temp ? tree->x : temp;

    temp = rt_1_1_move_onscreen(tree->c1);

    return min < temp ? min : temp;
}


void rt_2_petrify(node *tree, double offset)
{
    if(tree==NULL){
        return;
    }
    tree->x += offset;
    offset += tree->offset;

    rt_2_petrify(tree->c0, offset);
    rt_2_petrify(tree->c1, offset);
}


