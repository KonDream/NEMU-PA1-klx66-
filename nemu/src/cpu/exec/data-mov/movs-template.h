#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_n_, SUFFIX))
{
    MEM_W(reg_l(cpu.edi), MEM_R(reg_l(cpu.esi)));
    //swaddr_write(reg_l(R_EDI), 4, swaddr_read(reg_l(R_ESI), 4));
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