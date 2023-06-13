#include "am.h"
#include "common.h"
#include "proc.h"
extern _RegSet* do_syscall(_RegSet *r);
static _RegSet* do_event(_Event e, _RegSet* r) {
  static int num=0;
  switch (e.event) {
    case _EVENT_SYSCALL:
      //printf("System Call\n");
      do_syscall(r);
      return NULL;
      break;
    case _EVENT_TRAP:
      //Log("hello");
      return schedule(r);
      break;
    case _EVENT_IRQ_TIME:
      return NULL;
      break;
    default: panic("Unhandled event ID = %d", e.event);
  }
  num++;
  if(num==100){
    num=-1;
    return schedule(r);
  }
  if(num==0) return schedule(r);
  return NULL;
}

void init_irq(void) {
  _asye_init(do_event);
}
