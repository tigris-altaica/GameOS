struct Coordinates {
	unsigned int x;
	unsigned int y;
};

void PlayTetris(void);
void FixFigure(void);
void CreateNewFigure(void);
void GameOver(void);
void SetupScreen(void);
void HideFigure(void);
void ShowFigure(void);
int NotInFigure(unsigned int y, unsigned int x);
void MoveLeft(void);
void MoveRight(void);
int MoveDown(void);
void MoveDownToBottom(void);
void RotateFigure(void);
void Start(void);
void Exit(void);