
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>

#include "keysgame.h"

/* char exit_var=0; */

void (__interrupt __far *OldInt9)(void);


char Keys[KEYSNUM];
int Key;


void __interrupt __far NewInt9(void)
{ register char x;
  Key = inp(0x60);
/*  printf("%d\n", key);*/
  x=inp(0x61);
  outp(0x61, (x|0x82));
  outp(0x61,x);
  outp(0x20,0x20);

  switch(Key) {
     case 77:   Keys[K_RIGHT]=1; break;
     case 205:  Keys[K_RIGHT]=0; break;

     case 75:   Keys[K_LEFT]=1; break;
     case 203:  Keys[K_LEFT]=0; break;

     case 72:   Keys[K_UP]=1; break;
     case 200:  Keys[K_UP]=0; break;

     case 80:   Keys[K_DOWN]=1; break;
     case 208:  Keys[K_DOWN]=0; break;

  }

/*  if(key==129) exit_var=1;
  if(key==131) _clearscreen(_GCLEARSCREEN);*/
}



void init_keys()
{
  unsigned int i;
  for(i=0;i<KEYSNUM;i++) Keys[i]=0;
  OldInt9=_dos_getvect(0x9);
  _dos_setvect(0x9, NewInt9);
}

void de_init_keys()
{
  _dos_setvect(0x9, OldInt9);
}



/*
main()
{

  init_keys();
  for(;!exit_var;) ;
  de_init_keys();
  _setvideomode(_DEFAULTMODE);
}  */
