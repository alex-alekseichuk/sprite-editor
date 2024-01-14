
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <i86.h>


#include "sprite.h"
#include "keysgame.h"

/**********************************************************************
***********************************************************************
***********************************************************************/

#define S_XNUM  6
#define S_YNUM  4
#define S_XSIZE   50
#define S_YSIZE   50




void drawS()
{
  int x,y;
  UCHAR c;
  _clearscreen(_GCLEARSCREEN);
  _setcolor(1); _rectangle(_GFILLINTERIOR, 0,150,319,199);
  _setcolor(2); _moveto(0,149);_lineto(319,149);

} /* draw() */


#define MAX_LINEMAN_IMAGES 10
t_image *linemanl_i[10], *linemanr_i[10];
t_sprite *linemanl, *linemanr;
#define GETLINEMAN(c) (rmove?(linemanr_i[c]):(linemanl_i[c]))
/* Real LINE_MAN_HEIGHT + 1 = 43 + 1 =44 */
#define LINEMANHEIGHT 44

UCHAR rmove=1;
t_image *curhumanim;
char cadr=-1;
char hcx=1;
char hcy=2;
char hox=0;
char hoy=0;

char stepr[MAX_LINEMAN_IMAGES] = {1,2,0,3,13,3};
char stepl[MAX_LINEMAN_IMAGES] = {-4,-8,-7,-0,-5,2};
#define STEP(n) (rmove?(stepr[n]):(stepl[n]))


UCHAR linemanbuf[43*30];

void setcadr(char c)
{
  char ch=cadr;
  cadr=c; curhumanim=GETLINEMAN(c);
/*  switch (ch) {
    case -1 : return;
    case 0 : switch(c)

    } break;

  }*/
}



UCHAR linemanvis=0;
void drawhuman()
{
#define im curhumanim
  register int C=im->height*im->width;
  register int c=0,cb=0;
  register int X=hcx*S_XSIZE+hox,
               Y=(hcy+1)*S_YSIZE-hoy-LINEMANHEIGHT;

  if(linemanvis) return;
  X=Y=((Y<<8)+(Y<<6)+X-320);
  for(c=0;c<C;c++) {
    if(!(c%(im->width))) Y=(X+=320);
    if(im->buf[c]) {linemanbuf[cb++]=video_buf[Y]; video_buf[Y] = im->buf[c];}
    Y++;
  }
#undef im
  linemanvis=1;
}

void hidehuman()
{
#define im curhumanim
  register int C=im->height*im->width;
  register int c=0,cb=0;
  register int X=hcx*S_XSIZE+hox,
               Y=(hcy+1)*S_YSIZE-hoy-LINEMANHEIGHT;
/*  double ulo; */

  if(!linemanvis) return;

/*  for(ulo=0;ulo<2000;ulo+=.01); */

  X=Y=((Y<<8)+(Y<<6)+X-320);
  for(c=0;c<C;c++) {
    if(!(c%(im->width))) Y=(X+=320);
    if(im->buf[c]) video_buf[Y] = linemanbuf[cb++];
    Y++;
  }
#undef im
  linemanvis=0;
}

/*unsigned long MAY=0;
char May()
{
  if(ul)
} */

main()
{
  UCHAR uc, exit_var=0;
  t_image *im;
  unsigned long ul;

  if(!(linemanl=loadsprite("linemanl"))) exit(1);
  for(uc=0,im=linemanl->p ; uc<MAX_LINEMAN_IMAGES&&im ; im=im->next,uc++)
    linemanl_i[uc]=im;

  if(!(linemanr=loadsprite("linemanr"))) exit(1);
  for(uc=0,im=linemanr->p ; uc<MAX_LINEMAN_IMAGES&&im ; im=im->next,uc++)
    linemanr_i[uc]=im;
  setcadr(0);

  init_keys();
  _setvideomode(_MRES256COLOR);

  drawS();
  drawhuman();

  while(!exit_var) {

    if(Key==1) exit_var=1;

    switch(cadr) {
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
        for(ul=0;ul<3000000;ul++);
        hidehuman();
        if(cadr<5) {hox+=STEP(cadr); setcadr(cadr+1);}
        else if(((rmove&&Keys[K_RIGHT]&&!Keys[K_LEFT]) ||
                 (!rmove&&Keys[K_LEFT]&&!Keys[K_RIGHT])) &&
                 ((rmove&&hox<200) || (!rmove&&hox>30))
               ) {
                hox+=STEP(cadr);
                setcadr (6);
             } else {hox+=STEP(cadr); setcadr(0);}
        drawhuman();
        break;
      case 6:
        for(ul=0;ul<3000000;ul++);
        hidehuman();
        hox+=STEP(0); setcadr(1);
        drawhuman();
        break;


    } /* switch */

    if(Keys[K_RIGHT] && !Keys[K_LEFT]) {
      if(rmove) {
        switch(cadr) {
          case 0:
            if(hox<200) {
              hidehuman();
              setcadr(1);hox+=STEP(0);
              drawhuman();
            } break;

        } /* switch */
      } else {
        switch(cadr) {
          case 0:
            for(ul=0;ul<5000000;ul++);
            hidehuman(); rmove=1; setcadr(cadr); drawhuman();
            break;

        } /* switch */
      }
    } /* if(Right) */

    if(Keys[K_LEFT] && !Keys[K_RIGHT]) {
      if(!rmove) {
        switch(cadr) {
          case 0:
            if(hox>30) {
              hidehuman();
              setcadr(1);hox+=STEP(0);
              drawhuman();
            } break;

        } /* switch */
      } else {
        switch(cadr) {
          case 0:
            for(ul=0;ul<5000000;ul++);
            hidehuman(); rmove=0; setcadr(cadr); drawhuman();
            break;

        } /* switch */
      }
    } /* if(Left) */


  } /* while */

  de_init_keys();
  _setvideomode(_DEFAULTMODE);
} /* main() */
