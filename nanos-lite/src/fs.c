#include "fs.h"
#include "debug.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;


enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};
extern void fb_write(const void *buf, off_t offset, size_t len);
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))
extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern void ramdisk_write(const void *buf, off_t offset, size_t len);
extern void dispinfo_read(void *buf, off_t offset, size_t len);
extern size_t events_read(void *buf, size_t len);
int fs_open(const char *pathname, int flags, int mode){
    for(int i=3;i<NR_FILES;i++)
    if(strcmp(file_table[i].name,pathname)==0) {
      printf("file %d found\n",i);
      file_table[i].open_offset=0;
      return i;
    }
    panic("file not found");
    return -1;
}
ssize_t fs_read(int fd, void *buf, size_t len){
  assert((fd< NR_FILES));
    //printf("len0:%d\n",len);
    if(fd==FD_EVENTS){
      //Log("event\n");
      return events_read(buf,len);
    }
    len=min(file_table[fd].size-file_table[fd].open_offset,len);
    if(fd==FD_DISPINFO){
      dispinfo_read(buf, file_table[FD_DISPINFO].open_offset, len);
      file_table[FD_DISPINFO].open_offset+=len;
      return len;
    }
    //printf("len1:%d\n",len);
    ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
    file_table[fd].open_offset+=len;
    return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len){
  char* addr=(char*)buf;
  if(fd==1||fd==2){
      for(int i=0;i<len;i++) _putc(*(addr+i));
      return len;
  }else if(fd==3){
    fb_write(buf,file_table[fd].open_offset,len);
    file_table[fd].open_offset+=len;
    return len;
  }else{      
    assert((fd< NR_FILES));
    len=min(file_table[fd].size-file_table[fd].open_offset,len);
    ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
    file_table[fd].open_offset+=len;  
   return len;
  }
 
}
off_t fs_1seek(int fd, off_t offset, int whence){
  assert((fd< NR_FILES));
  if(whence==SEEK_SET) file_table[fd].open_offset=offset;
  if(whence==SEEK_CUR) file_table[fd].open_offset+=offset;
  if(whence==SEEK_END) file_table[fd].open_offset=offset+file_table[fd].size;
  //Log("%d",offset);
  return file_table[fd].open_offset;
}

int fs_close(int fd){
  return 0;
}
ssize_t fs_file_sz(int fd){
  //printf("%d\n",file_table[fd].size);
  return file_table[fd].size;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
  //get_screen_size(&x,&y);
  file_table[3].size=_screen.height*_screen.width;
}
