
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <bios.h>
#include <i86.h>

#include "mouse.h"
#include "sprite.h"

/**********************************************************************
***********************************************************************
***********************************************************************/

char filename[30]="new_image.256";
#define Width pim->width
#define Height pim->height
UCHAR G_w=10, G_h=10;
UCHAR DX=4;
int sWidth, sHeight;
#define _X 210

#define _XPal 210
#define _YPal 100
#define DXPal 6
unsigned int _X2Pal, _Y2Pal;

t_image *pim=NULL;

UCHAR curimagenum;
t_sprite *ps=NULL;

UCHAR curcolor=15, curbkcolor=0;

UCHAR ppflag=0;

int mb,mx,my;
UCHAR curx=100, cury, curbx=100, curby;

void Rectangle(UCHAR _x1, UCHAR _y1, UCHAR _x2, UCHAR _y2)
{
  UCHAR x1=MIN(_x1,_x2), x2=MAX(_x1,_x2), y1=MIN(_y1, _y2), y2=MAX(_y1, _y2);
  _rectangle(_GBORDER, x1*DX, y1*DX, (x2+1)*DX, (y2+1)*DX);
}

#include "some.c"





void putpixel(UCHAR x, UCHAR y, UCHAR c)
{
  if(!ps->p) return;
  MOUSE_OFF();
  if(ppflag) {
    int z;
    switch(ppflag) {
    case 1:
      for(z=my;z>=0;z--) {
        pim->buf[z*pim->width+mx]=c;
        _setcolor(c);
        _setpixel(_X+mx,z);
        _rectangle(_GFILLINTERIOR, mx*DX+1, z*DX+1, (mx+1)*DX-1, (z+1)*DX-1);
      } break;
    case 2:
      for(z=my;z<pim->height;z++) {
        pim->buf[z*pim->width+mx]=c;
        _setcolor(c);
        _setpixel(_X+mx,z);
        _rectangle(_GFILLINTERIOR, mx*DX+1, z*DX+1, (mx+1)*DX-1, (z+1)*DX-1);
      } break;
    case 3:
      for(z=mx;z>=0;z--) {
        pim->buf[my*pim->width+z]=c;
        _setcolor(c);
        _setpixel(_X+z,my);
        _rectangle(_GFILLINTERIOR, z*DX+1, my*DX+1, (z+1)*DX-1, (my+1)*DX-1);
      } break;
    case 4:
      for(z=mx;z<pim->width;z++) {
        pim->buf[my*pim->width+z]=c;
        _setcolor(c);
        _setpixel(_X+z,my);
        _rectangle(_GFILLINTERIOR, z*DX+1, my*DX+1, (z+1)*DX-1, (my+1)*DX-1);
      } break;
    case 5: { /* info */
      char s[5], str[10];
      itoa(mx+1, str, 10);strcat(str,",");
      itoa(my+1, s, 10);strcat(str,s);
      while(strlen(str)<5) strcat(str," ");
      _settextposition(9,30);
      _outtext(str);
      _setcolor(1);
      if(curx<100 && curbx<100) Rectangle(curx, cury, curbx, curby);
      if(c==curcolor) {
        if(curx<100) _rectangle(_GBORDER, curx*DX, cury*DX, (curx+1)*DX, (cury+1)*DX);
        curx=mx; cury=my; _setcolor(4);
        _rectangle(_GBORDER, curx*DX, cury*DX, (curx+1)*DX, (cury+1)*DX);
      }
      if(c==curbkcolor) {
        if(curbx<100) _rectangle(_GBORDER, curbx*DX, curby*DX, (curbx+1)*DX, (curby+1)*DX);
        curbx=mx; curby=my; _setcolor(2);
        _rectangle(_GBORDER, curbx*DX, curby*DX, (curbx+1)*DX, (curby+1)*DX);
      }
      if(curx<100 && curbx<100) {_setcolor(14);Rectangle(curx, cury, curbx, curby);}
    } break;

    }
    goto Exit_PP;
  }
  pim->buf[y*Width+x]=c;
  _setcolor(c);
  _setpixel(_X+x,y);
  _rectangle(_GFILLINTERIOR, x*DX+1, y*DX+1, (x+1)*DX-1, (y+1)*DX-1);
Exit_PP:
  MOUSE_ON();
}


void showscreen()
{
  int x,y;
  _setvideomode(_MRES256COLOR);
  _clearscreen(_GCLEARSCREEN);

  /* Init Screen Parameters */
  if(pim) {
    DX= MAX(Width, Height) ;
    DX = 200 / DX;

    sWidth  = Width * DX;
    sHeight = Height * DX;

    _setcolor(1);
    for(x=0;x<=sWidth;x+=DX) { _moveto(x,0);_lineto(x,sHeight); }
    for(y=0;y<=sHeight;y+=DX) { _moveto(0,y);_lineto(sWidth,y); }
    for(x=0;x<Width;x++)
      for(y=0;y<Height;y++) {
        _setcolor(pim->buf[y*Width+x]);_setpixel(_X+x,y);
        _rectangle(_GFILLINTERIOR, x*DX+1,y*DX+1,(x+1)*DX-1,(y+1)*DX-1);
      }
  }

  /* show pallete */
  for(x=0;x<16;x++)
    for(y=0;y<16;y++) {
      _setcolor(16*y+x);
      _rectangle(_GFILLINTERIOR, _XPal+x*DXPal, _YPal+y*DXPal,
                 _XPal+(x+1)*DXPal-1, _YPal+(y+1)*DXPal-1);

    }
  shownewcurcolor();
  showimagenum();
  MOUSE_ON();
} /* showscreen() */


main(int argc, char *argv[])
{
  int i;
  char sch[10];
  UCHAR w,h;

  if(argc>1) {
    if(toupper(argv[1][0])=='N') sch[0]='N';
    else {
      sch[0]='L';
      strcpy(filename, argv[1]);
    }
  }
  else {
    filename[0]='\0';
    printf("Your choice (New, Load, Quit) >");
    scanf("%s", sch);
  }
  switch(toupper(sch[0])){
    case 'Q' :
      printf("Ok. Bye.\n");
      exit(0);
    case 'N' :
      printf("File name :");scanf("%s", filename);
      printf("Width =");scanf("%d", &G_w);
      printf("Height =");scanf("%d", &G_h);
      if(G_h>MAX_IMAGE_SIZE) G_h=MAX_IMAGE_SIZE;
      if(G_w>MAX_IMAGE_SIZE) G_w=MAX_IMAGE_SIZE;

      ps=createsprite();
      pim=addimage(ps, G_w, G_h);
      curimagenum=1;
      break;
    case 'L' :
      if(filename[0]=='\0') {printf("File name :");scanf("%s", filename);}
      loadspritefromfile();
      break;


    default:
      printf("Wrong way. Bye.\n");
      exit(0);
  }

  INIT_MOUSE();

  _X2Pal = _XPal+16*DXPal-1;
  _Y2Pal = _YPal+16*DXPal-1;

  showscreen();

  i=0;
  while(1) {
    mb=GET_MOUSE(&mx, &my);
    if(kbhit()) switch(_bios_keybrd(_KEYBRD_READ)) {
      case 4209 : /* q */
        goto Exit_Program;
      case 8550 : /* f */
        newfilename(); break;
      case 7777: /* a */
        addimagetosprite(); break;
      case 12654: /* n */
        nextimage(); break;
      case 6512: /* p */
        previmage(); break;
      case 11875: /* c */
         copyimage(); break;
      case 11640: /* x */
        clearimage(); break;
      case 8292: /* d */
        delimagefromsprite(); break;
      case 8051: /* s */
        if(ps->p) savesprite(filename, ps);
        break;
      case 9836: /* l */
        loadspritefromfile(); break;
      case 5749: /* u */
        cutimage(); break;
      case 9064: /* h */
        hmirror(); break;
      case 12150: /* v */
        vmirror(); break;
      case 11386: /* z */
        switchppflag(); break;
      case 9322: /* j */
        joinrow(); break;



    }/* switch() */
    if(!mb) continue;
    mx/=2;

    /* SField */
    if(mx<sWidth && my<sHeight) {
      mx/=DX; my/=DX;
      if(mb==1) {
        putpixel(mx, my, curcolor);
      }
      if(mb==2) {
        putpixel(mx, my, curbkcolor);
      }
    } /* SField */

    /* SPallete */
    if(mx>=_XPal && mx<=_X2Pal && my>=_YPal && my<=_Y2Pal) {
      mx-=_XPal; my-=_YPal; mx/=DXPal; my/=DXPal;
      if(mb==1){
        curcolor=my*16+mx;
        shownewcurcolor();
      }
      if(mb==2) {
        curbkcolor=my*16+mx;
        shownewcurcolor();
      }
    } /* Spalette */

  }

Exit_Program:
  MOUSE_OFF();
  _setvideomode(_DEFAULTMODE);
  delsprite(ps);
} /* main() */
