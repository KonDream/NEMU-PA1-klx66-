#include "cpu/exec/template-start.h"

#define instr call

make_helper(concat(call_i_, SUFFIX))
{
    int length = concat(decode_i_, SUFFIX) (eip + 1);
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP), 4, cpu.eip + length);
    cpu.eip += op_src->val;
    print_asm("call %x", cpu.eip + 1 + length);
    return length + 1;
}

make_helper(concat(call_rm_, SUFFIX))
{
    int length = concat(decode_rm_, SUFFIX) (eip + 1);
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP), 4, cpu.eip + length);
    DATA_TYPE_S dis = op_src->val;
    print_asm("call %x", dis);
    cpu.eip = dis - length - 1;
    return length + 1;
}

#include "cpu/exec/template-end.h"