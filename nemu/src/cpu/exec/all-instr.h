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
make_EHelper(idiv);
make_EHelper(mul);
make_EHelper(div);
make_EHelper(neg);
//data-mov
make_EHelper(mov);
make_EHelper(pop);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(leave);
make_EHelper(cltd);
make_EHelper(cwtl);
make_EHelper(pusha);
make_EHelper(popa);
//control
make_EHelper(call);
make_EHelper(ret);
make_EHelper(jcc);
make_EHelper(jmp);
make_EHelper(jmp_rm);
make_EHelper(call_rm);
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
make_EHelper(rol);
make_EHelper(lidt);
//system
make_EHelper(in);
make_EHelper(out);
make_EHelper(iret);
make_EHelper(int);


