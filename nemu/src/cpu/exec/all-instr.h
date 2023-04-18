#include "cpu/exec.h"

make_EHelper(nop);

//arith
make_EHelper(sub);
make_EHelper(sbb);
make_EHelper(add);
make_EHelper(adc);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(cmp);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);
//data-mov
make_EHelper(mov);
make_EHelper(pop);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(leave);

//control
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);

//logic
make_EHelper(xor);
make_EHelper(or);
make_EHelper(and);
make_EHelper(test);
make_EHelper(setcc);
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(shr);
make_EHelper(not);
//special
make_EHelper(operand_size);
make_EHelper(inv);
make_EHelper(nemu_trap);

