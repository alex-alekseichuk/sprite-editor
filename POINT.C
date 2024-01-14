
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <graph.h>
#include <conio.h>
#include <bios.h>
#include <dos.h>
#include <i86.h>


#include "keysgame.h"

/**********************************************************************
***********************************************************************
***********************************************************************/

#define DD .0001

double x=100, y=100;


main()
{
  char exit_var=0;

   init_keys();
  _setvideomode(_MRES256COLOR);

  _setcolor(14);

  while(!exit_var) {
    if(Key==1) exit_var=1;
    if(Keys[K_RIGHT]) x+=DD;
    if(Keys[K_LEFT]) x-=DD;
    if(Keys[K_UP]) y-=DD;
    if(Keys[K_DOWN]) y+=DD;
    _setpixel((int)x,(int)y);

  }

  de_init_keys();
  _setvideomode(_DEFAULTMODE);
} /* main() */
