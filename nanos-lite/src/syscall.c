#include "am.h"
#include "arch.h"
#include "common.h"
#include "syscall.h"
#include "memory.h"
#include <bits/stdint-uintn.h>
#include <sys/types.h>
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
    //printf("%d\n",a[1]);
    _halt(a[1]);
    break;
  case SYS_write:
    //printf("SYS_write\n");
    Log("print %d chars\n",a[3]);
    if(a[1]==1||a[1]==2){
      char* addr=(char*) a[2];
      for(int i=0;i<a[3];i++) _putc(*(addr+i));
      SYSCALL_ARG1(r) = a[3];
    }
    break;
  case SYS_brk:
    while(0);
    static bool if_first=true;
    uint32_t pg_break=0;
    if(if_first){
      pg_break = *((uint32_t*)_heap.end);
      if_first=false;
    }else{
      pg_break+=a[1];
    }
    SYSCALL_ARG1(r)=0;
    break;

  default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return r;
}
