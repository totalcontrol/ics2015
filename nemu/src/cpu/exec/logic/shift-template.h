#include "cpu/exec/template-start.h"

static void concat(do_sar_, SUFFIX) () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);

	print_asm("sar" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
}

static void concat(do_shl_, SUFFIX) () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest <<= count;
	OPERAND_W(op_dest, dest);

	print_asm("shl" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
}

static void concat(do_shr_, SUFFIX) () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);

	print_asm("shr" str(SUFFIX) " %s,%s", op_src->str, op_dest->str);
}

#define template_shift \
make_instr_helper(rm_1) \
make_instr_helper(rm_cl) \
make_instr_helper(rm_imm)

#define instr sar
template_shift
#undef instr

#define instr shl
template_shift
#undef instr

#define instr shr
template_shift
#undef instr

#undef template_shift

#define do_shrd concat(do_shrd_, SUFFIX)

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_shrd() {
	DATA_TYPE in = op_dest->val;
	DATA_TYPE out = op_src2->val;

	uint8_t count = op_src->val;
	count &= 0x1f;
	while(count != 0) {
		out >>= 1;
		out |= (in & 1) << ((DATA_BYTE << 3) - 1);
		in >>= 1;
		count --;
	}

	OPERAND_W(op_src2, out);

	print_asm("shrd" str(SUFFIX) " %s,%s,%s", op_src->str, op_dest->str, op_src2->str);
}

make_helper(concat(shrdi_, SUFFIX)) {
	int len = concat(decode_si_rm2r_, SUFFIX) (eip + 1);  /* use decode_si_rm2r to read 1 byte immediate */
	op_dest->val = REG(op_dest->reg);
	do_shrd();
	return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
