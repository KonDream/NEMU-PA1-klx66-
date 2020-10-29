#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
    DATA_TYPE ans = op_dest->val & op_src->val;
    int length = DATA_BYTE * 8;
    cpu.EFLAGS.SF = ans >> (length - 1);
    if(ans)
    cpu.EFLAGS.ZF = 0;
    else
    cpu.EFLAGS.ZF = 1;
    cpu.EFLAGS.CF = 0;
    cpu.EFLAGS.OF = 0;
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.EFLAGS.PF = !(ans & 1);
    print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"