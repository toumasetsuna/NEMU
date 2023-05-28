#include "common.h"
#include "klib.h"

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t len) {
  return 0;
}

static char dispinfo[128] __attribute__((used));

void dispinfo_read(void *buf, off_t offset, size_t len) {
  Log("dispinfo_read");
  memcpy(buf, dispinfo+offset, len);
}
extern uint32_t* const fb;;
void fb_write(const void *buf, off_t offset, size_t len) {
  Log("fb_write");
  memcpy(fb+offset,buf,len);
}

void init_device() {
  _ioe_init();
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d",_screen.width,_screen.height);
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
