#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE ans = op_dest->val - op_src->val;
    OPERAND_W(op_dest, ans);
    int length = (DATA_BYTE << 3) - 1;
    int k1 = (op_dest->val) >> length;
    int k2 = (op_src->val) >> length;
    cpu.ZF = !ans;
    cpu.SF = ans >> length;
    cpu.CF = op_dest->val < op_src->val;
    cpu.OF = (k1 != k2 && k2 == cpu.SF);
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.PF = !(ans & 1);
    print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"