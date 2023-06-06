#include "common.h"
extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern size_t get_ramdisk_size();
void* new_page(void) ;
#define DEFAULT_ENTRY ((void *)0x8048000)
extern ssize_t fs_read(int fd, void *buf, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern ssize_t fs_file_sz(int fd);
 /* int fd=fs_open(filename,0,0);
  fs_read(fd, DEFAULT_ENTRY, fs_file_sz(fd));*/
uintptr_t loader(_Protect *as, const char *filename) { 
  //ramdisk_read(DEFAULT_ENTRY,0,get_ramdisk_size());
  int fd=fs_open(filename,0,0);
  int len=fs_file_sz(fd);
  _switch(as);
  for(int i=0;i<len;i+=4096){
    void* p=new_page();
    int len1= min(4096,len-i) ;
    _map(as,DEFAULT_ENTRY+i,p);
    fs_read(fd, DEFAULT_ENTRY+i, len1);
  }
  
  return (uintptr_t)DEFAULT_ENTRY;
}
