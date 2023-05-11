#include "types.h"
#include "screen.h"
#include "io.h"
#include "tetris.h"

struct Coordinates figCoord[4];

int color;

void InitScreen()
{
	char screen[VRAM_SIZE];
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

	printf(screen);
}

void CreateNewFigure()
{
	color = 0xe;

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

	/*for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = i - (i > 2);
		figCoord[i].x = MAX_COL / 2 + (i > 2);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y = i - (i > 1);
		figCoord[i].x = MAX_COL / 2 + (i > 1);
	}*/

	for (size_t i = 0; i < 4; i++)
	{
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}

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
		out_chr(' ', figCoord[i].y, figCoord[i].x, color);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y++;
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}

	return 0;
}

void MoveDownToBottom()
{
	while (MoveDown() != -1);
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
		out_chr(' ', figCoord[i].y, figCoord[i].x, color);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].x--;
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}
}

void MoveRight()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figCoord[i].x >= MAX_COL - 3){
			return;
		}
	}

	for (size_t i = 0; i < 4; i++)
	{
		out_chr(' ', figCoord[i].y, figCoord[i].x, color);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].x++;
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}
}

void StopMoving()
{
	int flag = 0;
	int lines_to_remove = 0;
	unsigned char* video_buf = (unsigned char*) DEF_VRAM_BASE;

	for (int curr_row = MAX_ROW - 1; curr_row >= 0; curr_row--)
	{
		for (int curr_col = 1; curr_col < MAX_COL - 2; curr_col++)
		{
			if ((video_buf + MAX_COL * 2 * curr_row + 2 * curr_col) != '#') {
				flag = 1;
				break;
			}
		}

		if (flag) {
			if (lines_to_remove == 0) {
				return;
			}
			else {
				break;
			}
		}
		else {
			lines_to_remove++;
		}
    }

	for (int curr_row = MAX_ROW - 1; curr_row > 0; curr_row--)
	{
		strncpy(video_buf + curr_row * 2 * MAX_COL,
		 	video_buf + (curr_row - lines_to_remove) * 2 * MAX_COL,
		  	2 * MAX_COL);
	}
}

void Exit()
{
	out16(0x604, 0x2000);
}

void PlayTetris()
{
	InitScreen();
	CreateNewFigure();

	while(1)
	{
		usleep(500000);
		int res = MoveDown();
		if (res == -1) {
			StopMoving();
			CreateNewFigure();
		} 
	}
}