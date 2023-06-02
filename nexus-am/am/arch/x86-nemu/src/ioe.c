#include <am.h>
#include <readline/readline.h>
#include <x86.h>

#define RTC_PORT 0x48   // Note that this is not standard
#define I8042_DATA_PORT 0x60
#define I8042_STATUS_PORT 0x64
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
  return inl(RTC_PORT) - boot_time;
}

uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};
void  get_screen_size(int *x,int *y){
  *x=_screen.width;
  *y=_screen.height;
}
static inline int min(int x, int y) {
  return (x < y) ? x : y;
}
void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
  /* for(int i=0;i<=h;i++){
    memcpy(fb+(y+i)*_screen.width+x,pixels+i*w,4*w);
   }*/
  int cp_bytes = sizeof(uint32_t) * min(w, _screen.width - x);
  for (int j = 0; j < h && y + j < _screen.height; j ++) {
    memcpy(&fb[(y + j) * _screen.width + x], pixels, cp_bytes);
    pixels += w;
  }

}
void _draw_fb(off_t offset,const char* buf, size_t len){
  memcpy(fb+(offset/4),buf,len);
}
void _draw_sync() {
}

int _read_key() {
  if (inb(I8042_STATUS_PORT) == 1) {
    return inl(I8042_DATA_PORT);
  }
  return _KEY_NONE;
}
