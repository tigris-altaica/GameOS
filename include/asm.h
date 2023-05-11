/*
 * Copyright (c) 2007, 2008 University of Tsukuba
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Tsukuba nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

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