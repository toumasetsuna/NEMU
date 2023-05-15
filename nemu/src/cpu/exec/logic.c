#include "cpu/decode.h"
#include "cpu/exec.h"
#include "cpu/reg.h"
#include "cpu/rtl.h"

make_EHelper(test) {
  rtl_and(&id_dest->val,&id_src->val,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  cpu.eflag.CF=0;
  cpu.eflag.OF=0;

  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&id_dest->val,&id_src->val,&id_dest->val);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  cpu.eflag.CF=0;
  cpu.eflag.OF=0;
  print_asm_template2(and);
}

make_EHelper(xor) {
  //rtl_xor(&id_dest->val,&id_src->val,&id_dest->val);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  cpu.eflag.CF=0;
  cpu.eflag.OF=0;
  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&id_dest->val,&id_src->val,&id_dest->val);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  cpu.eflag.CF=0;
  cpu.eflag.OF=0;

  print_asm_template2(or);
}

make_EHelper(sar) {
  rtl_sext(&id_dest->val,&id_dest->val,id_dest->width);
  rtl_sar(&t0,&id_dest->val,&id_src->val);
  operand_write(id_dest,&t0);
  rtl_update_ZFSF(&t0,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
 rtl_shl(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(shl);
  // unnecessary to update CF and OF in NEMU
}

make_EHelper(shr) {
  rtl_shr(&t0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t0);
  rtl_update_ZFSF(&t0, id_dest->width);
  print_asm_template2(shr);
  // unnecessary to update CF and OF in NEMU
}

make_EHelper(setcc) {
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not) {
  rtl_not(&id_dest->val);
   operand_write(id_dest, &id_dest->val);
  print_asm_template1(not);
}
