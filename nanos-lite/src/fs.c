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
int fs_open(const char *pathname, int flags, int mode){
    for(int i=3;i<NR_FILES;i++)
    if(strcmp(file_table[i].name,pathname)==0) {
      printf("file %d found\n",i);
      return i;
    }
    panic("file not found");
    return 0;
}
int min(ssize_t x,ssize_t y){
  return x<y?x:y;
}
ssize_t fs_read(int fd, void *buf, size_t len){
    printf("len0:%d\n",len);
    len=min(file_table[fd].size-(file_table[fd].open_offset+file_table[fd].disk_offset),len);
    printf("len1:%d\n",len);
    ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
    file_table[fd].open_offset+=len;
    return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len){
   len=min(file_table[fd].size-(file_table[fd].open_offset+file_table[fd].disk_offset),len);
  ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,len);
  file_table[fd].open_offset+=len;  
  return len;
}
off_t fs_1seek(int fd, off_t offset, int whence){
  if(whence==SEEK_SET) file_table[fd].open_offset=offset;
  if(whence==SEEK_CUR) file_table[fd].open_offset+=offset;
  if(whence==SEEK_END) file_table[fd].open_offset=offset+file_table[fd].size;
  return file_table[fd].open_offset;
}

int fs_close(int fd){
  return 0;
}
ssize_t fs_file_sz(int fd){
  return file_table[fd].size;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
