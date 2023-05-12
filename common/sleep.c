#include "asm.h"
#include "io.h"
#include "sleep.h"
#include "types.h"


/* read count of timer 0 */
static u16
get_timer0 (void)
{
	u16 count;
	u8 l, h;

	out8 (PIT_CONTROL,
		  PIT_CONTROL_BINARY |
		  PIT_CONTROL_MODE2 |
		  PIT_CONTROL_LATCH |
		  PIT_CONTROL_COUNTER0);
	in8 (PIT_COUNTER0, &l);
	in8 (PIT_COUNTER0, &h);
	count = l | (h << 8);
	return count;
}

/* busyloop for usec microseconds */
void
usleep (u32 usec)
{
	u32 count, remainder, counter0, tmp, diff;
	/*u64 acpitime, acpitime2;

	if (get_acpi_time (&acpitime)) {
		while (get_acpi_time (&acpitime2))
			if (acpitime2 - acpitime >= usec)
				return;
	}*/
	while (usec > 3599591843U) {
		usleep (3599591843U);
		usec -= 3599591843U;
	}
	asm_mul_and_div (14318181, usec, 12000000, &count, &remainder);
	if (remainder >= 12000000 / 2)
		count++;
	counter0 = get_timer0 ();
	while (count) {
		do
			tmp = get_timer0 ();
		while (counter0 == tmp);
		if (counter0 > tmp) {
			diff = counter0 - tmp;
			if (count < tmp)
				diff = count;
		} else {
			diff = 1;
		}
		count -= diff;
		counter0 = tmp;
	}
}
