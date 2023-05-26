#include "am.h"
#include "common.h"
#include "syscall.h"
#include "memory.h"
_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);
  switch (a[0]) {
  case SYS_none:
    SYSCALL_ARG1(r) = 1;
    printf("SYS_none\n");
    break;
  case SYS_exit:
    printf("SYS_exit\n");
    _halt(a[1]);
    break;
  case SYS_write:
    printf("SYS_write\n");
    if(a[1]==0||a[1]==1){
      char* addr=(char*) a[2];
      for(int i=0;i<a[3];i++) _putc(*(addr+i));
    }
    break;
  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return r;
}
