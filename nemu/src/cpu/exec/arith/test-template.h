#include "cpu/exec/template-start.h"

#define instr test



static void do_execute () {
	DATA_TYPE result,dest,src,mask8;
	uint8_t lsb8bits;
    uint8_t resultMsb;

	mask8= (DATA_TYPE)0x00000FF;
	
	dest= op_dest->val;   //val is always 32bits, so we must change format;
	src = op_src->val;
	result= dest&src;

	printf("test dest=%x  src=%x  result=%x\n",dest,src,result);
	
	
	/* TODO: Update EFLAGS. */
		//panic("please implement me");

	lsb8bits=(uint8_t)(result&mask8);
	//compute signed bits

	resultMsb=((result>>(sizeof(DATA_TYPE)*8-1))==0)?0:1;

    cpu.OF=0;
	cpu.CF=0;
	cpu.ZF=(result==0)?1:0;   //test ok
	//printf("DATA_TYPE__%d\n",sizeof(DATA_TYPE));
	cpu.SF=(resultMsb==1)?1:0; //test ok
    cpu.PF=(true==checkevenparity(lsb8bits))?1:0;  //test ok
	//assert(cpu.AF==0);

	print_asm_template1();
	
}


//function body,call idex(instruction decode and exec)
//make_instr_helper macro will generate 3 functions 
//test_i2a_b test_i2a_w test_i2a_l
make_instr_helper(i2a) 
make_instr_helper(i2rm)
make_instr_helper(r2rm)

//no byte instruction ,only word and 4 bytes
#if DATA_BYTE == 2 || DATA_BYTE == 4
//make_instr_helper(si2rm)
#endif



#include "cpu/exec/template-end.h"
