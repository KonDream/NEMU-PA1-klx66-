#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX))
{
    cpu.eip = MEM_R(reg_l(R_ESP)) - 2;
    reg_l(R_ESP) += DATA_BYTE;
    print_asm("ret\n");
    return 1;
}

/*make_helper(concat(ret_i_, SUFFIX))
{
    int addr = instr_fetch(eip + 1, 2);
    cpu.eip = MEM_R(REG(R_ESP));
    if(DATA_BYTE == 2)
    cpu.eip &= 0xffff;
    REG(R_ESP) += DATA_BYTE;
    int i;
    for(i = 0; i < addr; i += DATA_BYTE)
    MEM_W(REG(R_ESP) + i, 0);
    REG(R_ESP) += addr;
    print_asm("ret $0x%x", addr);
    return 1;
}*/

#include "cpu/exec/template-end.h"