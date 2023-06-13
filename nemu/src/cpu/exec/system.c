#include "cpu/decode.h"
#include "cpu/exec.h"
#include "cpu/reg.h"
#include "cpu/rtl.h"
#include <stdio.h>
extern void raise_intr(uint8_t NO, vaddr_t ret_addr);
void diff_test_skip_qemu();
void diff_test_skip_nemu();

make_EHelper(lidt) {
  //printf("hello lidt\n");
  rtl_lm(&t0,&id_dest->addr,2);
  rtl_addi(&t2, &id_dest->addr, 2);
  if(id_dest->width==4) rtl_lm(&t1, &t2, 4);
  if(id_dest->width==2) rtl_lm(&t1, &t2, 3);
  cpu.idtr.base=t1;
  cpu.idtr.limit=t0;
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //cpu.cr0.val=id_src->val;
  if(id_dest->reg==0) rtl_mv(&cpu.cr0.val,&id_src->val);
  if(id_dest->reg==3) rtl_mv(&cpu.cr3.val,&id_src->val);
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
  //Log("r2cr:%d",id_dest->reg);
  //if(id_dest->reg==0) Log("cr0: 0x%08x",cpu.cr0.val);
  //if(id_dest->reg==3) Log("cr3: 0x%08x",cpu.cr3.val);
}

make_EHelper(mov_cr2r) {
  if(id_src->reg==0) rtl_mv(&id_dest->val,&cpu.cr0.val);
  if(id_src->reg==3) rtl_mv(&id_dest->val,&cpu.cr3.val);
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));
  //Log("cr2r:%d",id_src->reg);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(int) {
  //printf("0x%x\n",id_dest->imm);
  raise_intr(id_dest->imm,decoding.seq_eip);

  print_asm("int %s", id_dest->str);

#ifdef DIFF_TEST
  //diff_test_skip_nemu();
#endif
}

make_EHelper(iret) {
  rtl_pop(&decoding.jmp_eip);
  decoding.is_jmp=1;
  rtl_pop(&cpu.CS);
  rtl_pop(&cpu.eflag);

  print_asm("iret");
}


uint32_t pio_read(ioaddr_t, int);
void pio_write(ioaddr_t, int, uint32_t);

make_EHelper(in) {
  id_dest->val=pio_read(id_src->val, id_dest->width);
  operand_write(id_dest, &id_dest->val);
  print_asm_template2(in);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}

make_EHelper(out) {
  pio_write(id_dest->val, id_dest->width, id_src->val);

  print_asm_template2(out);

#ifdef DIFF_TEST
  diff_test_skip_qemu();
#endif
}
