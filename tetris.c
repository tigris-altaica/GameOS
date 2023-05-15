#include "types.h"
#include "screen.h"
#include "io.h"
#include "sleep.h"
#include "tetris.h"

struct Coordinates figureCoord[4];
int figure;
int color;
int play;

int NotInFigure(unsigned int y, unsigned int x)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (figureCoord[i].x == x && figureCoord[i].y == y) {
			return 0;
		}
	}

	return 1;
}

void HideFigure()
{
	for (size_t i = 0; i < 4; i++)
	{
		out_chr(' ', figureCoord[i].y, figureCoord[i].x, color);
	}
}

void ShowFigure()
{
	for (size_t i = 0; i < 4; i++)
	{
		out_chr('#', figureCoord[i].y, figureCoord[i].x, color);
	}
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

	out_str((unsigned char *)"Game Over!", MAX_ROW / 2, MAX_COL / 2, 0x4);
}

void CreateNewFigure()
{
	color = rand() % 0xF + 1;

	figure = rand() % 7;

	switch (figure) {
	case 0: // I
		for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = 0;
			figureCoord[i].x = SCREEN_WIDTH / 2 - 1 + i;
		}
        break;
    case 1: // O
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = i % 2;
			figureCoord[i].x = (SCREEN_WIDTH + i) / 2;
		}
        break;
    case 2: // T
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = (i > 2);
			figureCoord[i].x = SCREEN_WIDTH / 2 - 1 + i - 2 * (i > 2);
		}
        break;
    case 3: // L
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = i - (i > 2);
			figureCoord[i].x = SCREEN_WIDTH / 2 + (i > 2);
		}
        break;
    case 4: // J
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = i - (i > 0);
			figureCoord[i].x = SCREEN_WIDTH / 2 + (i == 1);
		}
        break;
    case 5: // S
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = i - (i > 1);
			figureCoord[i].x = SCREEN_WIDTH / 2 + (i > 1);
		}
        break;
    case 6: // Z
        for (size_t i = 0; i < 4; i++)
		{
			figureCoord[i].y = i - (i > 1);
			figureCoord[i].x = SCREEN_WIDTH / 2 - (i > 1);
		}
        break;
    default:
    }

    for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR(figureCoord[i].y, figureCoord[i].x) == '#') {
			GameOver();
			return;
		}
	}

	ShowFigure();
}

int MoveDown()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR((figureCoord[i].y + 1), figureCoord[i].x) == '#'
			&& NotInFigure(figureCoord[i].y + 1, figureCoord[i].x)) {
			return -1;
		}

		if (figureCoord[i].y >= MAX_ROW - 1) {
			return -1;
		}
	}

	HideFigure();

	for (size_t i = 0; i < 4; i++)
	{
		figureCoord[i].y++;
	}

	ShowFigure();

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
		if (*LIN_ADDR(figureCoord[i].y, (figureCoord[i].x - 1)) == '#'
			&& NotInFigure(figureCoord[i].y, figureCoord[i].x - 1)) {
			return;
		}

		if (figureCoord[i].x <= 1){
			return;
		}
	}

	HideFigure();

	for (size_t i = 0; i < 4; i++)
	{
		figureCoord[i].x--;
	}

	ShowFigure();
}

void MoveRight()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (*LIN_ADDR(figureCoord[i].y, (figureCoord[i].x + 1)) == '#'
			&& NotInFigure(figureCoord[i].y, figureCoord[i].x + 1)) {
			return;
		}

		if (figureCoord[i].x >= SCREEN_WIDTH - 2){
			return;
		}
	}

	HideFigure();

	for (size_t i = 0; i < 4; i++)
	{
		figureCoord[i].x++;
	}

	ShowFigure();
}

void RotateFigure()
{
	struct Coordinates rotatedCoord[4];
	struct Coordinates offset = figureCoord[0];

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

	for (size_t i = 0; i < 4; i++)
	{
		rotatedCoord[i] = figureCoord[i];

		rotatedCoord[i].x -= offset.x;
		rotatedCoord[i].y -= offset.y;

		unsigned int temp = rotatedCoord[i].x;
		rotatedCoord[i].x = figureWidth - rotatedCoord[i].y - 1;
		rotatedCoord[i].y = temp;

		rotatedCoord[i].x += offset.x;
		rotatedCoord[i].y += offset.y;


		if (*LIN_ADDR(rotatedCoord[i].y, rotatedCoord[i].x) == '#'
			&& NotInFigure(rotatedCoord[i].y, rotatedCoord[i].x)) {
			return;
		}

		if (rotatedCoord[i].y >= MAX_ROW - 1) {
			return;
		}

		if (rotatedCoord[i].x >= SCREEN_WIDTH - 2){
			return;
		}
	}

	HideFigure();

	for (size_t i = 0; i < 4; i++)
	{
		figureCoord[i] = rotatedCoord[i];
	}

	ShowFigure();
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