#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_i_, SUFFIX))
{
    int length = concat(decode_i_, SUFFIX) (cpu.eip + 1) + 1;
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP), 4, cpu.eip + length);
    cpu.eip += op_src->val;
    print_asm("call %x", cpu.eip + 1 + length);
    return length + 1;
}

make_helper(concat(call_rm_, SUFFIX))
{
    int length = concat(decode_rm_, SUFFIX) (cpu.eip + 1);
    reg_l(R_ESP) -= DATA_BYTE;
    MEM_W(reg_l(R_ESP), cpu.eip + length);
    cpu.eip = (DATA_TYPE_S)op_src->val - length - 1;
    print_asm("call 0x%x\n", cpu.eip + length + 1);
    return length + 1;
}

#include "cpu/exec/template-end.h"