#include "cpu/exec.h"

//data-mov
make_EHelper(mov);
make_EHelper(pop);
make_EHelper(push);

//control
make_EHelper(call);

//special
make_EHelper(operand_size);
make_EHelper(inv);
make_EHelper(nemu_trap);

