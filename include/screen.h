#ifndef _SCREEN_H
#define _SCREEN_H

#define GREEN    0x2
#define MAX_COL  80		// Maximum number of columns 
#define MAX_ROW  25		// Maximum number of rows 
#define VRAM_SIZE (MAX_COL*MAX_ROW)	// Size of screen, in short's 
#define DEF_VRAM_BASE 0xb8000	// Default base for video memory

void clear_screen( void );
void putchar( int c );

#endif

