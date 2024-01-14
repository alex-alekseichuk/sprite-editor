
#ifndef __KEYSGAME__H__
#define __KEYSGAME__H__

#define K_RIGHT 0
#define K_LEFT  1
#define K_UP    2
#define K_DOWN  3

#define KEYSNUM 4

extern char Keys[KEYSNUM];
extern int Key;

extern void init_keys();
extern void de_init_keys();

#endif

