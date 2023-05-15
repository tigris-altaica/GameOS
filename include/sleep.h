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
