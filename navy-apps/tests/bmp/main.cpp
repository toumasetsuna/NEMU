#include <assert.h>
#include <stdlib.h>
#include <ndl.h>
#include <stdio.h>
int main() {
  NDL_Bitmap *bmp = (NDL_Bitmap*)malloc(sizeof(NDL_Bitmap));
  printf("hello 0\n");
  NDL_LoadBitmap(bmp, "/share/pictures/projectn.bmp");
  printf("hello 1\n");
  assert(bmp->pixels);
  NDL_OpenDisplay(bmp->w, bmp->h);
  printf("hello 2\n");
  NDL_DrawRect(bmp->pixels, 0, 0, bmp->w, bmp->h);
  printf("hello 3\n");
  NDL_Render();
  printf("hello r\n");
  printf("width:%d height:%d\n",bmp->w,bmp->h);
  NDL_CloseDisplay();
  while (1);
  return 0;
}
