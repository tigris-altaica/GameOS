#include "types.h"
#include "screen.h"
#include "io.h"
#include "tetris.h"

struct Coordinates figCoord[4];

int color;

int not_in_figure(unsigned int y, unsigned int x)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figCoord[i].x == x && figCoord[i].y == y) {
			return 0;
		}
	}

	return 1;
}

void InitScreen()
{
	color = 0x3;
	
	for (int curr_row = 0; curr_row < MAX_ROW; curr_row++)
	{
		out_chr('|', curr_row, 0, color);
		for (int curr_col = 1; curr_col < MAX_COL - 1; curr_col++)
		{
			out_chr(' ', curr_row, curr_col, color);
		}
		out_chr('|', curr_row, MAX_COL - 1, color);
	}
}

void CreateNewFigure()
{
	color = rand() % 0xF + 1;

	int figure = rand() % 5;

	switch (figure) {
	case 0:
		for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = 0;
			figCoord[i].x = MAX_COL / 2 - 1 + i;
		}
        break;
    case 1:
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i % 2;
			figCoord[i].x = (MAX_COL + i) / 2;
		}
        break;
    case 2:
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = (i > 2);
			figCoord[i].x = MAX_COL / 2 - 1 + i - 2 * (i > 2);
		}
        break;
    case 3:
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 2);
			figCoord[i].x = MAX_COL / 2 + (i > 2);
		}
        break;
    case 4:
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 1);
			figCoord[i].x = MAX_COL / 2 + (i > 1);
		}
        break;
    default:
    }

	for (size_t i = 0; i < 4; i++)
	{
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}

}

int MoveDown()
{
	for (size_t i = 0; i < 4; i++)
	{
		out_chr(' ', figCoord[i].y, figCoord[i].x, color);
	}

	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].y++;
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}

	for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR((figCoord[i].y + 1), figCoord[i].x) == '#'
			&& not_in_figure(figCoord[i].y + 1, figCoord[i].x)) {
			return -1;
		}

		if (figCoord[i].y >= MAX_ROW - 1) {
			return -1;
		}
	}

	return 0;
}

void MoveDownToBottom()
{
	while (MoveDown() != -1);
	StopMoving();
	CreateNewFigure();
}

void MoveLeft()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR(figCoord[i].y, (figCoord[i].x - 1)) == '#'
			&& not_in_figure(figCoord[i].y, figCoord[i].x - 1)) {
			return -1;
		}

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
		if (*LIN_ADDR(figCoord[i].y, (figCoord[i].x + 1)) == '#'
			&& not_in_figure(figCoord[i].y, figCoord[i].x + 1)) {
			return -1;
		}

		if (figCoord[i].x >= MAX_COL - 2){
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

	for (int curr_row = MAX_ROW - 1; curr_row >= 0; curr_row--)
	{
		for (int curr_col = 1; curr_col < MAX_COL - 1; curr_col++)
		{
			if (*LIN_ADDR(curr_row, curr_col) != '#') {
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
		memcpy(LIN_ADDR(curr_row, 1),
		 	LIN_ADDR((curr_row - lines_to_remove), 1),
		  	2 * (MAX_COL - 2));
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