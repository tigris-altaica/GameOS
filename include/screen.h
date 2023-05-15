#ifndef _SCREEN_H
#define _SCREEN_H

#define MAX_ROW  25		// Maximum number of rows 
#define MAX_COL  80		// Maximum number of columns 
#define SCREEN_WIDTH 30
#define VRAM_SIZE (MAX_COL*MAX_ROW)	// Size of screen, in short's 
#define VIDEO_BUFFER 0xb8000	// Default base for video memory

#define CURSOR_PORT (0x3D4) // Базовый порт управления курсором текстового экрана

#define LIN_ADDR(row, col) (unsigned char *)(VIDEO_BUFFER + MAX_COL * 2 * row + 2 * col)


void clear_screen(void);
void out_chr(const unsigned char chr, unsigned int row, unsigned int col, int color);
void out_str(unsigned char* str, unsigned int row, unsigned int col, int color);
void cursor_moveto(unsigned int strnum, unsigned int pos);

#endif

