#include "types.h"
#include "screen.h"
#include "io.h"
#include "tetris.h"

struct Coordinates figCoord[4];
int figure;
int color;
int play;

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

void SetupScreen()
{
	clear_screen();

	color = 0x3;
	
	for (int curr_row = 0; curr_row < MAX_ROW; curr_row++)
	{
		out_chr('|', curr_row, 0, color);
		for (int curr_col = 1; curr_col < SCREEN_WIDTH - 1; curr_col++)
		{
			out_chr(' ', curr_row, curr_col, color);
		}
		out_chr('|', curr_row, SCREEN_WIDTH - 1, color);
	}
}

void Start()
{
	SetupScreen();
	CreateNewFigure();

	play = 1;
}

void GameOver()
{
	play = 0;

	out_str("Game Over!", MAX_ROW / 2, MAX_COL / 2, 0x4);
}

void CreateNewFigure()
{
	color = rand() % 0xF + 1;

	figure = rand() % 7;

	switch (figure) {
	case 0: // I
		for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = 0;
			figCoord[i].x = SCREEN_WIDTH / 2 - 1 + i;
		}
        break;
    case 1: // O
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i % 2;
			figCoord[i].x = (SCREEN_WIDTH + i) / 2;
		}
        break;
    case 2: // T
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = (i > 2);
			figCoord[i].x = SCREEN_WIDTH / 2 - 1 + i - 2 * (i > 2);
		}
        break;
    case 3: // L
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 2);
			figCoord[i].x = SCREEN_WIDTH / 2 + (i > 2);
		}
        break;
    case 4: // J
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 0);
			figCoord[i].x = SCREEN_WIDTH / 2 + (i == 1);
		}
        break;
    case 5: // S
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 1);
			figCoord[i].x = SCREEN_WIDTH / 2 + (i > 1);
		}
        break;
    case 6: // Z
        for (size_t i = 0; i < 4; i++)
		{
			figCoord[i].y = i - (i > 1);
			figCoord[i].x = SCREEN_WIDTH / 2 - (i > 1);
		}
        break;
    default:
    }

    for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR(figCoord[i].y, figCoord[i].x) == '#') {
			GameOver();
			return;
		}
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
		if (*LIN_ADDR((figCoord[i].y + 1), figCoord[i].x) == '#'
			&& not_in_figure(figCoord[i].y + 1, figCoord[i].x)) {
			return -1;
		}

		if (figCoord[i].y >= MAX_ROW - 1) {
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
	FixFigure();
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

		if (figCoord[i].x >= SCREEN_WIDTH - 2){
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

void RotateFigure()
{
	for (size_t i = 0; i < 4; i++)
	{
		out_chr(' ', figCoord[i].y, figCoord[i].x, color);
	}

	struct Coordinates offset = figCoord[0];
	for (size_t i = 0; i < 4; i++)
	{
		figCoord[i].x -= offset.x;
		figCoord[i].y -= offset.y;

		unsigned int figureWidth;
		if (figure == 0){
			figureWidth = 4;
		}
		else if (figure == 1){
			figureWidth = 2;
		}
		else {
			figureWidth = 3;
		}

		unsigned int temp = figCoord[i].x;
		figCoord[i].x = figureWidth - figCoord[i].y - 1;
		figCoord[i].y = temp;

		figCoord[i].x += offset.x;
		figCoord[i].y += offset.y;
		
		out_chr('#', figCoord[i].y, figCoord[i].x, color);
	}
}

void FixFigure()
{
	int flag = 0;
	int lines_to_remove = 0;

	for (int curr_row = MAX_ROW - 1; curr_row >= 0; curr_row--)
	{
		for (int curr_col = 1; curr_col < SCREEN_WIDTH - 1; curr_col++)
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
		  	2 * (SCREEN_WIDTH - 2));
	}
}

void Exit()
{
	out16(0x604, 0x2000);
}

void PlayTetris()
{
	Start();

	while(1)
	{
		if (play)
		{
			usleep(500000);

			int ret = MoveDown();
			if (ret == -1) {
				FixFigure();
				CreateNewFigure();
			} 
		}
	}
}