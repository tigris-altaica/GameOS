#include "types.h"
#include "io.h"
#include "screen.h"

void out_chr(const unsigned char chr, unsigned int row, unsigned int col, int color) 
{  
    unsigned char* video_buf = (unsigned char*) VIDEO_BUFFER;  
    video_buf += MAX_COL * 2 * row + 2 * col; 
 
    video_buf[0] = chr;
    video_buf[1] = color;
}

void out_str(unsigned char* str, unsigned int row, unsigned int col, int color) 
{  
    while (*str)
    {
        out_chr(*str, row, col, color);
        str++;
        col++;
    }
} 

void cursor_moveto(unsigned int strnum, unsigned int pos) 
{     
    unsigned short new_pos = (strnum * MAX_COL) + pos;     
    out8(CURSOR_PORT, 0x0F);     
    out8(CURSOR_PORT + 1, (unsigned char)(new_pos & 0xFF));  
    out8(CURSOR_PORT, 0x0E);     
    out8(CURSOR_PORT + 1, (unsigned char)( (new_pos >> 8) & 0xFF)); 
}

void clear_screen( void )
{
    int i;
    for (i = 0; i < VRAM_SIZE; i++)
        out_chr(' ', i / MAX_COL, i % MAX_COL, 0x3);
}
