#include "Cards.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
BOOL GameInitialize(HINSTANCE hInstance)
{
	g_pGE = new GameEngine(
		hInstance, 
		g_strTitle, 
		g_strTitle, 
		IDI_BIGICON, 
		IDI_SMALLICON, 
		FALSE, 
		32, 
		800, 
		600
	);
	if (g_pGE == NULL)
	{
		return false;
	}
	g_pGE->SetFrameDelay(15);
	g_pGE->SetPause(false);
	if (!g_pGE->CreateGameWindow())
	{
		MessageBox(NULL, L"修改显示模式失败！\r\n程序退出", L"错误", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}
void GameStart(HWND hWnd)
{
	long lCur = (long)LoadCursor(g_pGE->GetInstance(), MAKEINTRESOURCE(IDC_MAINCURSOR));
	SetClassLong(hWnd, GCL_HCURSOR, lCur);
	int i, j, n, m;
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			g_rCardRect[i][j].left = CARSTARTX + (CARDWIDTH + COLSPACE) * j;
			g_rCardRect[i][j].right = g_rCardRect[i][j].left + CARDWIDTH;
			g_rCardRect[i][j].top = CARSTARTY + (CARDHEIGHT + ROWSPACE) * i;
			g_rCardRect[i][j].bottom = g_rCardRect[i][j].top + CARDHEIGHT;
			g_nCardState[i][j] = INITSELECT;
		}
	}
	srand((unsigned)time(NULL));
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < ROWS * COLS / 2; j++)
		{
			do {
				m = rand() % ROWS;
				n = rand() % COLS;
			} 
			while (g_nCardState[m][n] != INITSELECT);
			g_nCardPattern[m][n] = j;
			g_nCardState[m][n] = NOSELECT;
		}
	}
	g_nStartTime = timeGetTime();
	g_nTimeCount = 80000;
	SetRect(&g_rTimeBarFrame, 100, 150, 700, 170);
	g_nTimeBarLength = 0;
	g_nSelectRow1 = g_nSelectRow2 = g_nSelectCol1 = g_nSelectCol2 = -1;
	g_nSelected = 0;
}
void GameAction(HWND hWnd)
{
	long nCurrentTimes = timeGetTime() - g_nStartTime;
	if (nCurrentTimes < g_nTimeCount)
	{
		g_nTimeBarLength = nCurrentTimes * 596 / g_nTimeCount;
	}
	if (g_nSelectRow2 != -1)
	{
		if (
			g_nCardPattern[g_nSelectRow1][g_nSelectCol1]
			==
			g_nCardPattern[g_nSelectRow2][g_nSelectCol2]
			)
		{
			g_nCardState[g_nSelectRow1][g_nSelectCol1] = SELECTOK;
			g_nCardState[g_nSelectRow2][g_nSelectCol2] = SELECTOK;
			g_nCardPattern[g_nSelectRow1][g_nSelectCol1] = NULLPATTERN;
			g_nCardPattern[g_nSelectRow2][g_nSelectCol2] = NULLPATTERN;
			g_nSelected++;
			g_nSelectRow1 = g_nSelectRow2 = g_nSelectCol1 = g_nSelectCol2 = -1;
		}
		else
		{
			if (timeGetTime() - g_nTimeDelayStart > 200)
			{
				g_nCardState[g_nSelectRow1][g_nSelectCol1] = NOSELECT;
				g_nCardState[g_nSelectRow2][g_nSelectCol2] = NOSELECT;
				g_nSelectRow1 = g_nSelectRow2 = g_nSelectCol1 = g_nSelectCol2 = -1;
			}
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
	UpdateWindow(hWnd);
	if (nCurrentTimes < g_nTimeCount)
	{
		if (g_nSelected == 6)
		{
			if (
				MessageBox(
				g_pGE->GetWindow(),
				L"全部配对成功！你赢了！\r\n要继续游戏吗？",
				L"你赢了",
				MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK
				)
				== IDYES
				)
			{
				GameStart(hWnd);
			}
			else
			{
				DestroyWindow(hWnd);
			}
		}
	}
	else
	{
		if (
			MessageBox(
				g_pGE->GetWindow(),
				L"时间到！你输了！\r\n要继续游戏吗？",
				L"你输了",
				MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK
			)
			== IDYES
			)
		{
			GameStart(hWnd);
		}
		else
		{
			DestroyWindow(hWnd);
		}
	}
}
void MouseClick(HWND hWnd, int x, int y, WPARAM wParam)
{
	POINT pt = { x,y };
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			if (PtInRect(&g_rCardRect[i][j], pt))
			{
				if (g_nCardState[i][j] == NOSELECT && g_nSelectRow2 == -1)
				{
					if (g_nSelectRow1 == -1)
					{
						g_nSelectRow1 = i;
						g_nSelectCol1 = j;
					}
					else
					{
						g_nSelectRow2 = i;
						g_nSelectCol2 = j;
						g_nTimeDelayStart = timeGetTime();
					}
					g_nCardState[i][j] = SELECTED;
					return;
				}
			}
		}
	}
}
void GamePaint(HDC hDC)
{
	//Title
	RECT rBG;
	GetClientRect(g_pGE->GetWindow(), &rBG);
	HBRUSH hbrBG = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hDC, &rBG, hbrBG);
	DeleteObject(hbrBG);
	LOGFONT lf = { 0 };
	lf.lfHeight = 80;
	lf.lfWidth = 60;
	lf.lfWeight = 800;
	lf.lfCharSet = DEFAULT_CHARSET;
	wcscpy_s(lf.lfFaceName, L"黑体");
	HFONT hFont = CreateFontIndirect(&lf);
	HFONT hFontOld = (HFONT)SelectObject(hDC, hFont);
	TextOut(hDC, 100, 30, g_strTitle, wcslen(g_strTitle));
	SelectObject(hDC, hFontOld);
	DeleteObject(hFont);

	//Bar
	HBRUSH hbr = CreateSolidBrush(RGB(128, 128, 128));
	HBRUSH hbrOld = (HBRUSH)SelectObject(hDC, hbr);
	FrameRect(hDC, &g_rTimeBarFrame, hbr);
	Rectangle(
		hDC, 
		g_rTimeBarFrame.left + 2,
		g_rTimeBarFrame.top + 2,
		g_rTimeBarFrame.left + 2 + g_nTimeBarLength,
		g_rTimeBarFrame.bottom - 2
	);

	//Cards
	HBRUSH hbrBK = CreateHatchBrush(HS_BDIAGONAL, RGB(128, 128, 128));
	HBRUSH hbrCard;
	RECT r;
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			FrameRect(hDC, &g_rCardRect[i][j], hbr);
			r = g_rCardRect[i][j];
			if (g_nCardState[i][j] == NOSELECT)
			{
				InflateRect(&r, -10, -10);
				FillRect(hDC, &r, hbrBK);
			}
			else
			{
				InflateRect(&r, -20, -20);
				switch (g_nCardPattern[i][j])
				{
				case 0:
					hbrCard = CreateSolidBrush(RGB(128, 255, 0));
					SelectObject(hDC, hbrCard);
					RoundRect(hDC, r.left, r.top, r.right, r.bottom, 10, 10);
					SelectObject(hDC, hbrOld);
					break;
				case 1:
					hbrCard = CreateSolidBrush(RGB(255, 128, 255));
					SelectObject(hDC, hbrCard);
					RoundRect(hDC, r.left, r.top, r.right, r.bottom, 10, 10);
					SelectObject(hDC, hbrOld);
					break;
				case 2:
					hbrCard = CreateSolidBrush(RGB(255, 128, 128));
					SelectObject(hDC, hbrCard);
					Rectangle(hDC, r.left, r.top, r.right, r.bottom);
					SelectObject(hDC, hbrOld);
					break;
				case 3:
					hbrCard = CreateSolidBrush(RGB(0, 128, 128));
					SelectObject(hDC, hbrCard);
					Rectangle(hDC, r.left, r.top, r.right, r.bottom);
					SelectObject(hDC, hbrOld);
					break;
				case 4:
					hbrCard = CreateSolidBrush(RGB(128, 0, 255));
					SelectObject(hDC, hbrCard);
					Ellipse(hDC, r.left, r.top, r.right, r.bottom);
					SelectObject(hDC, hbrOld);
					break;
				case 5:
					hbrCard = CreateSolidBrush(RGB(255, 0, 128));
					SelectObject(hDC, hbrCard);
					Ellipse(hDC, r.left, r.top, r.right, r.bottom);
					SelectObject(hDC, hbrOld);
					break;
				default:
					hbrCard = (HBRUSH)GetStockObject(WHITE_BRUSH);
					SelectObject(hDC, hbrCard);
					FillRect(hDC, &g_rCardRect[i][j], hbrCard);
					SelectObject(hDC, hbrOld);
					break;
				}
				DeleteObject(hbrCard);
			}
		}
	}
	DeleteObject(hbrBK);
	DeleteObject(hbr);
}
void GameEnd()
{
	delete g_pGE;
}
void GameActive(HWND hWnd)
{
	g_pGE->SetPause(FALSE);
}
void GamePause(HWND hWnd)
{
	g_pGE->SetPause(TRUE);
}
BOOL GameWindowClose(HWND hWnd)
{
	if (MessageBox(
		g_pGE->GetWindow(),
		L"你确定要退出吗？",
		L"退出游戏",
		MB_YESNO | MB_DEFBUTTON2 | MB_ICONASTERISK
	)
		== IDYES)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}