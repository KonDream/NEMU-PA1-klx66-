#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute () {
	DATA_TYPE result = op_dest->val + op_src->val + cpu.EFLAGS.CF;
	OPERAND_W(op_dest, result);
	int length = (DATA_BYTE << 3) - 1;
    int k1 = (op_dest->val) >> length;
    int k2 = (op_src->val) >> length;
    cpu.EFLAGS.ZF = !result;
    cpu.EFLAGS.SF = result >> length;
    cpu.EFLAGS.CF = result < op_dest->val;
    cpu.EFLAGS.OF = (k1 == k2 && k1 != cpu.EFLAGS.SF);
    result ^= result >> 4;
    result ^= result >> 2;
    result ^= result >> 1;
    cpu.EFLAGS.PF = !(result & 1);
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
