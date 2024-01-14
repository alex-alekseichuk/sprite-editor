

void newfilename()
{
  _settextposition(0,0);
  printf("File Name : ");scanf("%s", filename);
  showscreen();
}

void showimagenum()
{
  char s[5];
  char str[10];
  _settextposition(10,32);
  if(!ps->n) {_outtext("No"); return; }
  itoa(curimagenum, str, 10);  str[3]=0;
  itoa(ps->n, s, 10);  s[3]=0;
  strcat(str,",");strcat(str,s);
  _outtext(str);

  _settextposition(11,32);
  itoa(pim->width, str, 10);  str[3]=0;
  itoa(pim->height, s, 10);  s[3]=0;
  strcat(str,",");strcat(str,s);
  _outtext(str);
}


void addimagetosprite()
{
  t_image *old;
  if(ps->n>=MAX_IMAGE_NUM) return;
  old=pim;
  if(pim=addimage(ps, pim?pim->width:G_w, pim?pim->height:G_h)) {
    curimagenum++;
    showscreen();
  } else pim=old;
}

void delimagefromsprite()
{
  t_image *p;
  if(!ps->p) return;
  if(!pim->next) {
    curimagenum--;
    if(ps->n==1) p=NULL;
    else for(p=ps->p;p->next!=pim;p=p->next);
  }
  else p=pim->next;
  delimage(ps, pim);
  pim=p;
  showscreen();
}

void clearimage()
{
  int i;
  if(!ps->p) return;
  for(i=0;i<pim->width*pim->height;i++) pim->buf[i]=curbkcolor;
  showscreen();
}

void nextimage()
{
  if(!ps->p) return;
  if(pim->next) { pim=pim->next; curimagenum++; showscreen(); }
}

void previmage()
{
  t_image *i=ps->p;
  if((!ps->p) || ps->p==pim) return;
  for(;i->next!=pim;i=i->next);
  pim=i; curimagenum--;
  showscreen();
}

void shownewcurcolor()
{
  _setcolor(1);
  _rectangle(_GBORDER, _XPal+10, _YPal-25, _XPal+30, _YPal-5);
  _setcolor(curbkcolor);
  _rectangle(_GFILLINTERIOR, _XPal+11, _YPal-24, _XPal+29, _YPal-6);

  _setcolor(1);
  _rectangle(_GBORDER, _XPal, _YPal-35, _XPal+20, _YPal-15);
  _setcolor(curcolor);
  _rectangle(_GFILLINTERIOR, _XPal+1, _YPal-34, _XPal+19, _YPal-16);
}

void copyimage()
{
  UCHAR n=0;
  unsigned int k;
  t_image *i;

  if(!ps->p) return;
  _settextposition(0,0);
  printf("Copy to Image N (0-New) :");scanf("%d", &n);
  if(!n || n>ps->n) {  /* create new */
    i=addimage(ps, pim->width, pim->height);
  } else {  /* change */
    i=ps->p;
    if(n>1) for(k=0;k<n-1;k++) i=i->next;
    free(i->buf);
    i->width=pim->width; i->height=pim->height;
    i->buf=(UCHAR *)calloc(i->width*i->height, 1);
  }
  for(k=0;k<i->width*i->height;k++) i->buf[k]=pim->buf[k];
  showscreen();
}



void loadspritefromfile()
{

  delsprite(ps);
  ps=loadsprite(filename);
  if(ps) {
    pim=ps->p;
    curimagenum=1;
    showscreen();
  }
}



void cutimage()
{
  UCHAR w, x0=MIN(curx,curbx), y0=MIN(cury,curby),
           x2=MAX(curx,curbx), y2=MAX(cury,curby), x, y, *buf;

  buf=(UCHAR *)calloc((x2-x0+1)*(y2-y0+1), 1);
  w=x2-x0+1;
  for(x=x0;x<=x2;x++)
    for(y=y0;y<=y2;y++)
      buf[(y-y0)*w+x-x0]=pim->buf[y*pim->width+x];
  free(pim->buf);
  pim->buf=buf;
  pim->width=w;
  pim->height=y2-y0+1;
  showscreen();
}


void hmirror()
{
  UCHAR x,y,z;
  if(!ps->p) return;
  for(x=0;x<pim->width/2;x++)
    for(y=0;y<pim->height;y++) {
      z=pim->buf[y*pim->width+x];
      pim->buf[y*pim->width+x]=pim->buf[y*pim->width+pim->width-1-x];
      pim->buf[y*pim->width+pim->width-1-x]=z;
    }
    showscreen();
}


void vmirror()
{
  UCHAR x,y,z;
  if(!ps->p) return;
  for(y=0;y<pim->height/2;y++)
    for(x=0;x<pim->width;x++) {
      z=pim->buf[y*pim->width+x];
      pim->buf[y*pim->width+x]=pim->buf[(pim->height-1-y)*pim->width+x];
      pim->buf[(pim->height-1-y)*pim->width+x]=z;
    }
  showscreen();
}



void switchppflag()
{
 int code=_bios_keybrd(_KEYBRD_READ);
  switch(code) {
    case 18432:
      ppflag=1; break;
    case 20480:
      ppflag=2; break;
    case 19200:
      ppflag=3; break;
    case 19712:
      ppflag=4; break;
    case 5993:
      ppflag=5; break;
    default:
      ppflag=0;
  }
}


void joinrow()
{

  int key=_bios_keybrd(_KEYBRD_READ);
  UCHAR *p;
  unsigned int c,y;

  switch(key) {
    case 19712:
      if(pim->width<MAX_IMAGE_SIZE) {
        p=calloc( (++(pim->width))*pim->height ,1);
        for(y=0;y<pim->height;y++) {
          for(c=0;c<pim->width-1;c++)
            p[y*pim->width+c]=pim->buf[y*(pim->width-1)+c];
          p[y*pim->width+pim->width-1]=0;
        }
        free(pim->buf);
        pim->buf=p;
        showscreen();
      } break;
    case 19200:
      if(pim->width<MAX_IMAGE_SIZE) {
        p=calloc( (++(pim->width))*pim->height ,1);
        for(y=0;y<pim->height;y++) {
          p[y*pim->width]=0;
          for(c=1;c<pim->width;c++)
            p[y*pim->width+c]=pim->buf[y*(pim->width-1)+c-1];
        }
        free(pim->buf);
        pim->buf=p;
        showscreen();
      } break;
    case 18432:
      if(pim->height<MAX_IMAGE_SIZE) {
        p=calloc( pim->width*(++(pim->height)) ,1);
        for(c=0;c<pim->width;c++) p[c]=0;
        for(;c<pim->width*pim->height;c++)
          p[c]=pim->buf[c-pim->width];
        free(pim->buf);
        pim->buf=p;
        showscreen();
      } break;

    case 20480:
      if(pim->height<MAX_IMAGE_SIZE) {
        p=calloc( pim->width*(++(pim->height)) ,1);
        for(c=0;c<pim->width*(pim->height-1);c++)
          p[c]=pim->buf[c];
        for(;c<pim->width*pim->height;c++) p[c]=0;
        free(pim->buf);
        pim->buf=p;
        showscreen();
      } break;
  }

}