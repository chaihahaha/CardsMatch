#ifndef GAMEENGINE
#define GAMEENGINE 1

#include "windows.h"
class GameEngine
{
protected:
	static GameEngine* m_pGameEngine;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	TCHAR m_szWindowClass[50];
	TCHAR m_szTitle[50];
	WORD m_wIcon;
	WORD m_wSamllIcon;
	int m_nWidth, m_nHeight;
	BOOL m_bFullScreen;
	int m_nColorbit;
	int m_nFrameDelay;
	long m_lNextGameTick;
	long m_lStartTime;
	BOOL m_bPause;
public:
	GameEngine(HINSTANCE hInstance,
		LPCTSTR szWindowClass,
		LPCTSTR szTitle,
		WORD wIcon,
		WORD wSmallIcon,
		BOOL bFullScreen = FALSE,
		int nColorbit = 32,
		int nWidth = 800,
		int nHeight = 600);
	virtual ~GameEngine();
	BOOL CreateGameWindow();
	static GameEngine* GetGameEngine()
	{
		return m_pGameEngine;
	};
	HINSTANCE GetInstance()
	{
		return m_hInstance;
	};
	HWND GetWindow()
	{
		return m_hWnd;
	};
	void SetWindow(HWND hWnd)
	{
		m_hWnd = hWnd;
	};
	BOOL GetPause()
	{
		return m_bPause;
	};
	void SetPause(BOOL bPause)
	{
		m_bPause = bPause;
	};
	int GetFrameDelay()
	{
		return m_nFrameDelay;
	};
	void SetFrameDelay(int nFrameDelay)
	{
		m_nFrameDelay = nFrameDelay;
	};
	int GetWidth()
	{
		return m_nWidth;
	};
	int GetHeight()
	{
		return m_nHeight;
	};
	long GetStartTime()
	{
		return m_lStartTime;
	};
	void SetNextGameTick(int nNextGameTick)
	{
		m_lNextGameTick = nNextGameTick;
	};
	int GetNextGameTick()
	{
		return m_lNextGameTick;
	}
};

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdline,
	int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);
BOOL GameInitialize(HINSTANCE hInstance);
void GameStart(HWND hWnd);
void GameEnd();
void GameAction(HWND hWnd);
void MouseClick(HWND hWnd,
	int x,
	int y,
	WPARAM wParam);
void GamePaint(HDC hWnd);
void GameActive(HWND hWnd);
void GamePause(HWND hWnd);
BOOL GameWindowClose(HWND hWnd);

#endif