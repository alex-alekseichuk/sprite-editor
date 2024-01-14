
#ifndef __SPRITE__H__
#define __SPRITE__H__

#define UCHAR unsigned char
#define MAX(X,Y) (((X)>(Y))?(X):(Y))
#define MIN(X,Y) (((X)<(Y))?(X):(Y))

#define MAX_IMAGE_SIZE 50

typedef struct s_image {
  UCHAR width, height;
  UCHAR *buf;
  struct s_image *next;
} t_image;


#define MAX_IMAGE_NUM 30

typedef struct s_sprite {
  UCHAR n;
  t_image *p;
} t_sprite;

extern t_image *createimage(UCHAR w, UCHAR h);
extern void delimage(t_sprite *p, t_image *pi);
extern t_sprite *createsprite();
extern t_image *addimage(t_sprite *p, UCHAR w, UCHAR h);
extern int savesprite(char *fn, t_sprite *p);
extern t_sprite *loadsprite(char *fn);
extern void delsprite(t_sprite *p);


extern UCHAR far *video_buf;
extern unsigned int far *video_buf_w;

extern void showimage(t_image *im, int X, int Y);
extern void showmagicimage(t_image *im, int X, int Y);


#endif