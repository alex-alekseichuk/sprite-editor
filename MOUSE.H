
#ifndef __MOUSE__H__
#define __MOUSE__H__

extern int MOUSE_FLAG;
extern int INIT_MOUSE();
extern void MOUSE_ON();
extern void MOUSE_OFF();
/*extern void MOUSE_BORDER(int,int,int,int);*/
extern int  GET_MOUSE(int *,int *);
/*extern void SET_MOUSE(int,int);*/

#endif