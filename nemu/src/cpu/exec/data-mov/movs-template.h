#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_n_, SUFFIX))
{
    current_sreg = R_DS;
    uint32_t tmp = MEM_R(reg_l(R_ESI));
    current_sreg = R_ES;
    swaddr_write(reg_l(R_EDI), DATA_BYTE, tmp);
    if(cpu.EFLAGS.DF == 0)
    {
        reg_l(R_EDI) += DATA_BYTE;
        reg_l(R_ESI) += DATA_BYTE;
    }
    else
    {
        reg_l(R_EDI) -= DATA_BYTE;
        reg_l(R_ESI) -= DATA_BYTE;
    }
    print_asm("movs\n");
    return 1;
}

#include "cpu/exec/template-end.h"