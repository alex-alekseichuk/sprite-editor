#include <bios.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

main()
{
  int key;

  while(1) {
    while(!kbhit());
    key=_bios_keybrd(_KEYBRD_READ);
    printf(" - %d -\n", key);
    if(key==283) exit(0);

  }
}