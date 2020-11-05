#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	cpu.EFLAGS.CF = 0;
	cpu.EFLAGS.OF = 0;
	cpu.EFLAGS.ZF = !result;
	cpu.EFLAGS.OF = result < op_src->val;
	cpu.EFLAGS.SF = result >> ((DATA_BYTE << 3) - 1);
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
