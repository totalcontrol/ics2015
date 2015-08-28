#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute () {
	//DATA_TYPE result,dest,src,mask8,mask4;
	
	op_dest->val=swaddr_read(cpu.esp,4);  //pop 
	cpu.esp=cpu.esp+sizeof(DATA_BYTE);   //move esp
	
	/* TODO: Update EFLAGS. */
		//panic("please implement me");

	/*lsb8bits=(uint8_t)(result&mask8);
	//compute signed bits
	srcMsb=((src>>(sizeof(DATA_TYPE)*8-1))==0)?0:1;
	destMsb=((dest>>(sizeof(DATA_TYPE)*8-1))==0)?0:1;	
	resultMsb=((result>>(sizeof(DATA_TYPE)*8-1))==0)?0:1;

    cpu.OF=(((~destMsb&srcMsb&resultMsb)|(destMsb&~srcMsb&~resultMsb))==1)?1:0;
	cpu.CF=(dest<src)?1:0;
	cpu.ZF=(result==0)?1:0;   //test ok
	//printf("DATA_TYPE__%d\n",sizeof(DATA_TYPE));
	cpu.SF=(resultMsb==1)?1:0; //test ok
    cpu.AF=(((dest&mask4)-(src&mask4))>mask4)?1:0;  // dec overflow, bcd compute	cpu.PF=()
    cpu.PF=(true==checkevenparity(lsb8bits))?1:0;  //test ok
	//assert(cpu.AF==0);*/

	print_asm_template1(); 
	printf("pop %x \n",op_src->val);
}


//function body,call idex(instruction decode and exec)
//make_instr_helper macro will generate 3 functions 
//sub_i2a_b sub_i2a_w sub_i2a_l
//make_instr_helper(i) 
make_instr_helper(r) 



//no byte instruction ,only word and 4 bytes
#if DATA_BYTE == 4
//make_instr_helper(i)
#endif



#include "cpu/exec/template-end.h"
