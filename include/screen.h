#ifndef _SCREEN_H
#define _SCREEN_H

#define SCREEN_WIDTH 30
#define MAX_COL  80		// Maximum number of columns 
#define MAX_ROW  25		// Maximum number of rows 
#define VRAM_SIZE (MAX_COL*MAX_ROW)	// Size of screen, in short's 
#define VIDEO_BUFFER 0xb8000	// Default base for video memory

#define LIN_ADDR(row, col) (unsigned char *)(VIDEO_BUFFER + MAX_COL * 2 * row + 2 * col)


void clear_screen( void );
void putchar( int c );

#endif

