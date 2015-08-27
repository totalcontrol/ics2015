#ifndef __REG_H__
#define __REG_H__

#include "common.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
union
{
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
		uint8_t _byte[4];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. */

	struct{uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;};
};
	swaddr_t eip;
	union
		{
	struct
		{
		 int :8;
		 int :6;
		 int VM:1; 
		 int RF:1;
		 int RES1:1;
		 int NT:1;
		 int IP:1;
		 int OL:1;
		 int OF:1;
		 int DF:1;
		 int IF:1;
		 int TF:1;
		 int SF:1;
		 int ZF:1;
		 int RES2:1;
		 int AF:1;
		 int RES3:1;
		 int PF:1;
		 int RES4:1;
		 int CF:1;
		};
	   uint32_t EFLAGS;
	} ;

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
