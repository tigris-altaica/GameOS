#include "types.h"
#include "screen.h"
#include "io.h"
#include "tetris.h"

struct Coordinates figCoord[4];

char screen[VRAM_SIZE];

void InitScreen()
{
	for (int curr_row = 0; curr_row < MAX_ROW; curr_row++)
	{
		screen[LIN_ADDR(curr_row, 0)] = '|';
		for (int curr_col = 1; curr_col < MAX_COL - 2; curr_col++)
		{
			screen[LIN_ADDR(curr_row, curr_col)] = ' ';
		}
		screen[LIN_ADDR(curr_row, MAX_COL - 2)] = '|';
		screen[LIN_ADDR(curr_row, MAX_COL - 1)] = '\n';
	}
}

void CreateNewFigure()
{
	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = 0;
		figCoord[i].x = MAX_COL / 2 - 1 + i;
	}

	/*for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = i % 2;
		figCoord[i].x = (MAX_COL + i) / 2;
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = (i > 2);
		figCoord[i].x = MAX_COL / 2 - 1 + i - 2 * (i > 2);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = i - (i > 2);
		figCoord[i].x = MAX_COL / 2 + (i > 2);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = i - (i > 1);
		figCoord[i].x = MAX_COL / 2 + (i > 1);
	}*/

}

int MoveDown()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figCoord[i].y >= MAX_ROW - 1){
			return -1;
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y++;
	}

	return 0;
}

void MoveLeft()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figCoord[i].x <= 1){
			return;
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].x--;
	}
}

void MoveRight()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figCoord[i].x >= MAX_COL - 2){
			return;
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].x++;
	}
}

void StopMoving()
{
	for (size_t i = 0; i < 4; i++)
	{
		screen[LIN_ADDR(figCoord[i].y, figCoord[i].x)] = '#';
	}

	int lines_to_remove = 0;
	for (int curr_row = MAX_ROW - 1; curr_row <= 0; curr_row--)
	{
		for (int curr_col = 1; curr_col < MAX_COL - 2; curr_col++)
		{
			if (screen[LIN_ADDR(curr_row, curr_col)] != '#')
				return;
		}
		lines_to_remove++;
    }

	for (int curr_row = MAX_ROW - 1; curr_row <= 0; curr_row--)
	{
		strncpy(screen + curr_row * MAX_COL,
		 	screen + (curr_row - lines_to_remove) * MAX_COL,
		  	MAX_COL);
	}
}

void DrawScreen()
{
	char screen_copy[VRAM_SIZE];
	strncpy(screen_copy, screen, VRAM_SIZE);

	for (size_t i = 0; i < 4; i++)
	{
		screen_copy[LIN_ADDR(figCoord[i].y, figCoord[i].x)] = '#';
	}	

	printf(screen_copy);
}

void Exit()
{
	out16(0x604, 0x2000);
}

void PlayTetris()
{
	InitScreen();

	CreateNewFigure();
	DrawScreen();

	while(1)
	{
		//Sleep(1);
		//int res = MoveDown();
		//if (res == -1) {
		//	StopMoving();
		//	CreateNewFigure();
		//}
		//DrawScreen();
		asm("hlt");  
	}
}