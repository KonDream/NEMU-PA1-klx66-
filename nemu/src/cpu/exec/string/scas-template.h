#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX))
{
    uint32_t ans = REG(R_EAX) - MEM_R(reg_l(R_EDI));
    if(cpu.EFLAGS.DF == 0)
    reg_l(R_EDI) += DATA_BYTE;
    else
    reg_l(R_EDI) -= DATA_BYTE;
    int len = (DATA_BYTE << 3) - 1;
    cpu.EFLAGS.ZF = !ans;
    cpu.EFLAGS.SF = ans >> len;
    cpu.EFLAGS.CF = REG(R_EAX) < MEM_R(reg_l(R_EDI));
    int k1 = REG(R_EAX) >> len;
    int k2 = MEM_R(reg_l(R_EDI)) >> len;
    cpu.EFLAGS.OF = (k1 != k2 && k2 == cpu.EFLAGS.SF);
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.EFLAGS.PF = !(ans & 1);
    print_asm("scas%s", str(SUFFIX));
    return 1;
}

#include "cpu/exec/template-end.h"