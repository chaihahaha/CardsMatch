#include "GameEngine.h"
#include "resource.h"
GameEngine* g_pGE;
enum SELECTSTATE {
	INITSELECT=0,
	NOSELECT,
	SELECTED,
	SELECTOK
};
const int NULLPATTERN = -1;
const int CARDWIDTH = 100;
const int CARDHEIGHT = 100;
const int ROWS = 3;
const int COLS = 4;
const int ROWSPACE = 20;
const int COLSPACE = 50;
const int CARSTARTX = 120;
const int CARSTARTY = 200;
const wchar_t* g_strTitle = L"卡片对对碰";
int g_nCardPattern[ROWS][COLS];
RECT g_rCardRect[ROWS][COLS];
SELECTSTATE g_nCardState[ROWS][COLS];
int g_nSelectRow1, g_nSelectRow2, g_nSelectCol1, g_nSelectCol2;
int g_nSelected;
RECT g_rTimeBarFrame;
int g_nTimeBarLength;
int g_nTimeCount;
int g_nStartTime;
long g_nTimeDelayStart;
