#include<conio.h>
#include<graph.h>
#include<i86.h>


unsigned int MOUSE_FLAG;
/*extern char INIT_MOUSE();
extern void MOUSE_ON();
extern void MOUSE_OFF();
extern void MOUSE_BORDER(int,int,int,int);
extern int  GET_MOUSE(int *,int *);
extern void SET_MOUSE(int,int);
*/


unsigned int INIT_MOUSE()
{
  union REGS regs;
  regs.w.ax=0;
  int86(0x33, &regs, &regs);
  MOUSE_FLAG=regs.w.ax;
  return regs.w.ax;
}


void MOUSE_ON()
{
  union REGS regs;
  regs.w.ax=1;
  int86(0x33, &regs, &regs);
}

void MOUSE_OFF()
{
  union REGS regs;
  regs.w.ax=2;
  int86(0x33, &regs, &regs);
}

int GET_MOUSE(int *x, int *y)
{
  union REGS regs;
  regs.w.ax=3;
  int86(0x33, &regs, &regs);
  (*x)=regs.w.cx;
  (*y)=regs.w.dx;
  return regs.w.bx;
}

/*
char q;int gd=0,gm,x,y,b;

void main(void){
_clearscreen(_GCLEARSCREEN);
q=INIT_MOUSE();
MOUSE_ON();

while(!kbhit()){
  while(!(b=GET_MOUSE(&x,&y)));
  MOUSE_OFF();
  printf("----- %d  %d  %d\n", b,x,y);
  MOUSE_ON();
}

MOUSE_OFF();
getch();
exit(0);}
  */