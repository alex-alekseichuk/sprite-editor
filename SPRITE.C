

#include <stdio.h>
#include <stdlib.h>
#include "sprite.h"


t_image *createimage(UCHAR w, UCHAR h)
{
  t_image *pi;
  unsigned int i;
  pi=(t_image *)malloc(sizeof(t_image));
  if(pi) {
    pi->next=NULL;
    pi->width=w;
    pi->height=h;
    pi->buf=(UCHAR *)calloc(w*h, 1);
    if(!pi->buf) { free(pi);pi=NULL; }
    for(i=0;i<w*h;i++) pi->buf[i]=0;
  }
  return pi;
} /* createimage() */

void delimage(t_sprite *p, t_image *pi)
{
  if(pi && p) {
    if(p->p==pi) p->p=pi->next;
    else {
      t_image *i=p->p;
      for(;i->next!=pi;i=i->next);
      i->next=pi->next;
    }
    if(pi->buf) free(pi->buf);
    free(pi);
    p->n--;
  }
}


t_sprite *createsprite()
{
  t_sprite *p;
  p=(t_sprite *)malloc(sizeof(t_sprite));
  if(p) {
    p->n=0;
    p->p=NULL;
  }
  return p;
}

t_image *addimage(t_sprite *p, UCHAR w, UCHAR h)
{
  t_image *pi;
  if(!(pi=createimage(w,h))) return NULL;
  p->n++;
  if(!p->p) p->p=pi;
  else {
    t_image *i=p->p;
    for(;i->next;i=i->next);
    i->next=pi;
  }
  return pi;
}


int savesprite(char *fn, t_sprite *p)
{
  FILE *f;
  t_image *i;
  unsigned int k;

  if(!p || !(f=fopen(fn, "wt"))) return 0;

  fprintf(f, "%2.2d", p->n);
  for(i=p->p;i;i=i->next) {
    fprintf(f, "%2.2d%2.2d", i->width, i->height);
    for(k=0;k<i->width*i->height;k++)
      fprintf(f, "%c", i->buf[k]);
  }
  fclose(f);
  return 1;
}


t_sprite *loadsprite(char *fn)
{
  FILE *f;
  t_image *i;
  unsigned int k;
  t_sprite *p;
  UCHAR n, w, h, g;

  p=createsprite();
  if(!p || !(f=fopen(fn, "rt"))) return NULL;

  if(!feof(f)) fscanf(f, "%2d", &n);
/*  printf("N=%d\n", n);*/
  for(g=0;g<n;g++) {
    fscanf(f, "%2d%2d", &w, &h);
/*    printf(" -%d-  w=%d  h=%d\n", g+1, w, h);*/
    i=addimage(p, w, h);
    for(k=0;k<i->width*i->height;k++)
      {fscanf(f, "%c", &(i->buf[k]));
/*       if(!(k%i->width)) printf("\n");
       printf("%d ", i->buf[k]);*/

      }
  }
  fclose(f);
  return p;
}

        void delsprite(t_sprite *p)
{
  if(p) {
    if(p->p) {
      t_image *i, *j;
      for(i=p->p;i;) {
        free(i->buf);
        j=i->next;free(i);i=j;
      }

    }
    free(p);
  }
}

UCHAR far *video_buf = (char far *)0xA0000000L;
unsigned int far *video_buf_w = (int far *)0xA0000000L;


void showimage(t_image *im, int X, int Y)
{
  register int C=im->height*im->width;
  register int c=0;

  X=Y=((Y<<8)+(Y<<6)+X-320);
  for(c=0;c<C;c++) {
    if(!(c%(im->width))) Y=(X+=320);
    video_buf[Y] = im->buf[c];
    Y++;
  }
}


void showmagicimage(t_image *im, int X, int Y)
{
  register int C=im->height*im->width;
  register int c=0;

  X=Y=((Y<<8)+(Y<<6)+X-320);
  for(c=0;c<C;c++) {
    if(!(c%(im->width))) Y=(X+=320);
    if(im->buf[c]) video_buf[Y] = im->buf[c];
    Y++;
  }
}


/*
main()
{
  t_sprite *ps;

  ps=loadsprite("n");


}
*/


