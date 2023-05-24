#include "cpu/decode.h"
#include "cpu/exec.h"
#include "cpu/reg.h"
#include "cpu/rtl.h"
#include "memory/memory.h"
#include "memory/mmu.h"
#include <sys/types.h>

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflag);
  rtl_push(&cpu.CS);
  rtl_push(&cpu.eip);
  rtl_mv(&t0,&cpu.idtr.base);
  rtl_lm(&t1,&t0,2);
  t0+=6;
  rtl_lm(&t2, &t0, 2);
  t2=(t1<<16)+t2;
  decoding.jmp_eip = t2;
  decoding.is_jmp=true;
}

void dev_raise_intr() {
}
