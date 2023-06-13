#include "common.h"
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
  cpu.eflag.IF=0;
  rtl_push(&cpu.CS);
  rtl_push(&ret_addr);
  rtl_mv(&t0,&cpu.idtr.base);
  t0+=8*NO;
 /* rtl_lm(&t1,&t0,2);
  t0+=6;
  rtl_lm(&t2, &t0, 2);
  t2=(t2<<16)+t1;*/
  GateDesc g;
  ((vaddr_t*) &g)[0]=vaddr_read(t0,4);
  ((vaddr_t*) &g)[1]=vaddr_read(t0+4,4);
  t2=g.offset_15_0+(g.offset_31_16<<16);
  //printf("t2:0x%x\n",t2);
  decoding.jmp_eip = t2;
  decoding.is_jmp=true;
}

void dev_raise_intr() {
   cpu.INTR=true;
}
