#ifndef _CORE_ASM_H
#define _CORE_ASM_H

#include "types.h"

enum invvpid_type {
	INVVPID_TYPE_INDIVIDUAL_ADDRESS = 0,
	INVVPID_TYPE_SINGLE_CONTEXT = 1,
	INVVPID_TYPE_ALL_CONTEXTS = 2,
	INVVPID_TYPE_SINGLE_CONTEXT_EXCEPT_GLOBAL = 3,
};

enum invept_type {
	INVEPT_TYPE_SINGLE_CONTEXT = 1,
	INVEPT_TYPE_ALL_CONTEXTS = 2,
};


#define SW_SREG_ES_BIT (1 << 0)
#define SW_SREG_CS_BIT (1 << 1)
#define SW_SREG_SS_BIT (1 << 2)
#define SW_SREG_DS_BIT (1 << 3)
#define SW_SREG_FS_BIT (1 << 4)
#define SW_SREG_GS_BIT (1 << 5)

/* 0f 01 c1                vmcall */
#ifdef AS_DOESNT_SUPPORT_VMX
#	define ASM_VMCALL ".byte 0x0F, 0x01, 0xC1"
#else
#	define ASM_VMCALL "vmcall"
#endif

static inline void
asm_mul_and_div (u32 mul1, u32 mul2, u32 div1, u32 *quotient, u32 *remainder)
{
	asm ("mull %4 ; divl %5"
	     : "=&a" (*quotient)
	     , "=&d" (*remainder)
	     : "0" (mul1)
	     , "1" (0)
	     , "rm" (mul2)
	     , "rm" (div1)
	     : "cc");
}

#endif