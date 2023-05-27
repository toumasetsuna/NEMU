#include "common.h"
extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern size_t get_ramdisk_size();
#define DEFAULT_ENTRY ((void *)0x4000000)
extern ssize_t fs_read(int fd, void *buf, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern ssize_t fs_file_sz(int fd);
uintptr_t loader(_Protect *as, const char *filename) { 
  int fd=fs_open(filename,0,0);
  fs_read(fd, DEFAULT_ENTRY, fs_file_sz(fd));
  return (uintptr_t)DEFAULT_ENTRY;
}
