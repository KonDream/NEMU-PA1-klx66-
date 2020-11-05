#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	int len = (DATA_BYTE << 3) - 1;
	int k1 = op_src->val >> len;
	int k2 = 0;
	cpu.EFLAGS.ZF = !result;
	cpu.EFLAGS.SF = result >> len;
	cpu.EFLAGS.OF = (k1 != k2 && k2 == cpu.EFLAGS.SF);
	result ^= result >> 4;
	result ^= result >> 2;
	result ^= result >> 1;
	cpu.EFLAGS.PF = !(result & 1);
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
