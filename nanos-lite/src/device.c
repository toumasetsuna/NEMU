#include "common.h"
#include "klib.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};
size_t events_read(void *buf, size_t len) {
  if(len==0) return 0;
  int len_1=0;
  {
    int x=_read_key();

    if(x==_KEY_F12){
      change_game();
      _trap();
    }
    if(x!=_KEY_NONE){
      sprintf(buf,"%s %s\n",x&0x8000?"kd":"ku",keyname[x&~0x8000]);
    }else{
      unsigned long t=_uptime();
     sprintf(buf,"t %d\n",t);
    }
    while(((char*)buf)[len_1++]!='\n');
    return (len<len_1)?len:len_1;
    
 }
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  Log("dispinfo_read");
  Log("%d",offset);
  memcpy(buf, dispinfo+offset, len);
}
void fb_write(const void *buf, off_t offset, size_t len) { 
  _draw_fb(offset,buf,len);
} 
  

void init_device() {
  _ioe_init();
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n",_screen.width,_screen.height);
  printf("WIDTH:%d\nHEIGHT:%d\n",_screen.width,_screen.height);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
