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

#ifndef _CORE_SLEEP_H
#define _CORE_SLEEP_H

#include "types.h"

#define PIT_COUNTER0            0x40
#define PIT_COUNTER1            0x41
#define PIT_COUNTER2            0x42
#define PIT_CONTROL         0x43
#define PIT_CONTROL_BINARY      0x0
#define PIT_CONTROL_BCD         0x1
#define PIT_CONTROL_MODE0       0x0
#define PIT_CONTROL_MODE1       0x2
#define PIT_CONTROL_MODE2       0x4
#define PIT_CONTROL_MODE3       0x6
#define PIT_CONTROL_MODE4       0x8
#define PIT_CONTROL_MODE5       0xA
#define PIT_CONTROL_LATCH       0x00
#define PIT_CONTROL_LOW8BIT     0x10
#define PIT_CONTROL_HIGH8BIT        0x20
#define PIT_CONTROL_16BIT       0x30
#define PIT_CONTROL_COUNTER0        0x00
#define PIT_CONTROL_COUNTER1        0x40
#define PIT_CONTROL_COUNTER2        0x80
#define PIT_CONTROL_READBACK        0xC0

void waitcycles (u32 d, u32 a);
void usleep (u32 usec);
void sleep_set_timer_counter (void);

#endif
