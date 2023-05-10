#define LIN_ADDR(y, x) (y * MAX_COL + x)

struct Coordinates {
	unsigned int x;
	unsigned int y;
};

void PlayTetris(void);