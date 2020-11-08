#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE ans = op_dest->val | op_src->val;
	OPERAND_W(op_dest, ans);

	/* TODO: Update EFLAGS. */
	//panic("please implement me");
	cpu.EFLAGS.CF = 0;
	cpu.EFLAGS.OF = 0;
	cpu.EFLAGS.ZF = !ans;
    cpu.EFLAGS.SF = MSB(ans);
    ans ^= ans >> 4;
    ans ^= ans >> 2;
    ans ^= ans >> 1;
    cpu.EFLAGS.PF = !(ans & 1);
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
