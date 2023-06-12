#include "am.h"
#include "common.h"
#include "proc.h"
extern _RegSet* do_syscall(_RegSet *r);
static _RegSet* do_event(_Event e, _RegSet* r) {
  switch (e.event) {
    case _EVENT_SYSCALL:
      //printf("System Call\n");
      do_syscall(r);
      break;
    case _EVENT_TRAP:
      Log("hello");
      schedule(r);
      break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  _asye_init(do_event);
}
