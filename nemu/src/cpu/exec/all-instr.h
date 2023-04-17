#include "cpu/exec.h"

make_EHelper(nop);
//arith
make_EHelper(sub);
make_EHelper(sbb);
make_EHelper(add);
make_EHelper(adc);
//data-mov
make_EHelper(mov);
make_EHelper(pop);
make_EHelper(push);
make_EHelper(lea);
make_EHelper(achg);
//control
make_EHelper(call);
make_EHelper(ret);

//logic
make_EHelper(xor);
make_EHelper(or);
make_EHelper(and);

//special
make_EHelper(operand_size);
make_EHelper(inv);
make_EHelper(nemu_trap);

