#include "types.h"
#include "screen.h"

static unsigned char curr_col = 0;
static unsigned char curr_row = 0;

// Write character at current screen location
#define PUT(c) ( ((unsigned short *) (VIDEO_BUFFER)) \
	[(curr_row * MAX_COL) + curr_col] = (0x2 << 8) | (c))

// Place a character on next screen position
static void cons_putc(int c)
{
    switch (c) 
    {
    case '\t':
        do 
        {
            cons_putc(' ');
        } while ((curr_col % 8) != 0);
        break;
    case '\r':
        curr_col = 0;
        break;
    case '\n':
        curr_row += 1;
        if (curr_row >= MAX_ROW) 
        {
            curr_row = 0;
        }
        break;
    case '\b':
        if (curr_col > 0) 
        {
            curr_col -= 1;
            PUT(' ');
        }
        break;
    default:
        PUT(c);
        curr_col += 1;
        if (curr_col >= MAX_COL) 
        {
            curr_col = 0;
            curr_row += 1;
            if (curr_row >= MAX_ROW) 
            {
                curr_row = 0;
            }
        }
    };
}

void putchar( int c )
{
    if (c == '\n') 
        cons_putc('\r');
    cons_putc(c);
}

void clear_screen( void )
{
    curr_col = 0;
    curr_row = 0;
    
    int i;
    for (i = 0; i < VRAM_SIZE; i++)
        cons_putc(' ');
    
    curr_col = 0;
    curr_row = 0;
}
