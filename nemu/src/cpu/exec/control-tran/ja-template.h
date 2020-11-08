#include "cpu/exec/template-start.h"

#define instr ja

static void do_execute()
{
    DATA_TYPE_S tmp = op_src->val;
    print_asm("ja %x\n", cpu.eip + tmp + DATA_BYTE + 1);
    if(cpu.EFLAGS.CF == 0 && cpu.EFLAGS.ZF == 0)
    cpu.eip += tmp;
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"