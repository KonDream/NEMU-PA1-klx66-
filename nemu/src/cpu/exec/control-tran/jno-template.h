#include "cpu/exec/template-start.h"

#define instr jno

static void do_execute()
{
    DATA_TYPE_S tmp = op_src->val;
    print_asm("jno %x\n", cpu.eip + tmp + DATA_BYTE + 1);
    if(cpu.EFLAGS.OF == 0)
    cpu.eip += tmp;
}

make_instr_helper(i);

#include "cpu/exec/template-end.h"