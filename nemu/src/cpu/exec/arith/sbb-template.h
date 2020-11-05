#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute()
{
    DATA_TYPE ans;
    if(op_src->size == 1 && op_dest->size != 1)
        ans = op_dest->val - ((int32_t)(op_src->val) + cpu.EFLAGS.CF);
    else
        ans = op_dest->val - (op_src->val + cpu.EFLAGS.CF);
    OPERAND_W(op_dest, ans);
   /* int len = (DATA_BYTE << 3) - 1;
    int k1 = op_dest->val >> len;
    int k2 = op_src->val >> len;
    cpu.EFLAGS.ZF = !ans;
    cpu.EFLAGS.SF = ans >> len;
    cpu.EFLAGS.CF = op_dest->val < (op_src->val + cpu.EFLAGS.CF);
    cpu.EFLAGS.OF = (k1 != k2 && k1 == cpu.EFLAGS.SF);
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.EFLAGS.PF = !(ans & 1);*/
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm);
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"