#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute()
{
    DATA_TYPE_S tmp = op_src->val;
    if(op_src->type == OP_TYPE_IMM)
    cpu.eip += tmp;
    else
    {
        cpu.eip = tmp - concat(decode_rm_, SUFFIX)(cpu.eip + 1) - 1;

    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)

#if DATA_BYTE == 4

extern Sreg_Descriptor *sreg_desc;
Sreg_Descriptor new_sreg_desc;
make_helper(ljmp){
    sreg_desc = &new_sreg_desc;

    cpu.eip = instr_fetch(cpu.eip + 1, 4) - 7;

    uint16_t idx = cpu.cs.selector >> 3;//index of sreg

	lnaddr_t chart_addr = cpu.gdtr.base + (idx << 3);//chart addr
    
	sreg_desc->part1 = lnaddr_read(chart_addr, 4);
	sreg_desc->part2 = lnaddr_read(chart_addr + 4, 4);

    cpu.cs.base = sreg_desc->base1 + (sreg_desc->base2 << 16) + (sreg_desc->base3 << 24);
    cpu.cs.limit =  sreg_desc->limit1 + (sreg_desc->limit2 << 16) + (0xfff << 24);


	if (sreg_desc->g == 1) cpu.cs.limit <<= 12;//G = 0, unit = 1B;G = 1, unit = 4KB
    print_asm("ljump %x %x",instr_fetch(cpu.eip+1 + 4, 2),instr_fetch(cpu.eip+1, 4));
    return 7;    
}
#endif

#include "cpu/exec/template-end.h"