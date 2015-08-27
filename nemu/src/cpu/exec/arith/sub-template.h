#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute () {
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	panic("please implement me");

	print_asm_template1();
}


//function body,call idex(instruction decode and exec)
//make_instr_helper macro will generate 3 functions 
//sub_i2a_b sub_i2a_w sub_i2a_l
make_instr_helper(i2a) 
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

//no byte instruction ,only word and 4 bytes
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif



#include "cpu/exec/template-end.h"
