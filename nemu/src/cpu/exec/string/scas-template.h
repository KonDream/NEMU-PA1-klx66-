#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_, SUFFIX))
{
    current_sreg = R_ES;
    swaddr_t k = REG(R_EAX);
    swaddr_t l = MEM_R(reg_l(R_EDI));
    uint32_t ans = k - l;
    if(cpu.EFLAGS.DF == 0)
    reg_l(R_EDI) += DATA_BYTE;
    else
    reg_l(R_EDI) -= DATA_BYTE;
    int len = (DATA_BYTE << 3) - 1;
    cpu.EFLAGS.ZF = !ans;
    cpu.EFLAGS.SF = ans >> len;
    cpu.EFLAGS.CF = k < l;
    int k1 = k >> len;
    int k2 = l >> len;
    cpu.EFLAGS.OF = (k1 != k2 && k2 == cpu.EFLAGS.SF);
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.EFLAGS.PF = !(ans & 1);
    print_asm("scas%s", str(SUFFIX));
    return 1;
}

#include "cpu/exec/template-end.h"