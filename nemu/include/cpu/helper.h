#ifndef __HELPER_H__
#define __HELPER_H__

#include "nemu.h"
#include "cpu/decode/operand.h"

/* All function defined with 'make_helper' return the length of the operation. */
#define make_helper(name) int name(swaddr_t eip)

static inline uint32_t instr_fetch(swaddr_t addr, size_t len) {
	return swaddr_read(addr, len);
}

/* Instruction Decode and EXecute */
static inline int idex(swaddr_t eip, int (*decode)(swaddr_t), void (*execute) (void)) {
	/* eip is pointing to the opcode */
	int len = decode(eip + 1);   //fetch operator ,get dest and src
	execute();
	return len + 1;	// "1" for opcode
}

//add by tiger
static inline bool checkevenparity(uint8_t x)
{
  bool even=true;
   int i=0;
  while (x)
  	{
  	  even=(x&1)?false:true;
	  printf("x=%8x___i=%d___even=%d\n",x,i++,even);

	  x=x>>1;
  	}
  return even;
  	
}

/* shared by all helper function */
extern Operands ops_decoded;

#define op_src (&ops_decoded.src)
#define op_src2 (&ops_decoded.src2)
#define op_dest (&ops_decoded.dest)


#endif
