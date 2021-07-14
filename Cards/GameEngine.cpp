#include "GameEngine.h"
#include "winuser.h"
#include "mmsystem.h"
#include "wingdi.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

GameEngine* GameEngine::m_pGameEngine = NULL;
GameEngine::GameEngine(HINSTANCE hInstance,
	LPCTSTR szWindowClass,
	LPCTSTR szTitle,
	WORD wIcon,
	WORD wSmallIcon,
	BOOL bFullScreen,
	int nColorbit,
	int nWidth,
	int nHeight)
{
	m_pGameEngine = this;
	m_hInstance = hInstance;
	lstrcpy(m_szWindowClass, szWindowClass);
	lstrcpy(m_szTitle, szTitle);
	m_bFullScreen = bFullScreen;
	m_nColorbit = nColorbit;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_wIcon = wIcon;
	m_wSamllIcon = wSmallIcon;
	m_lStartTime = timeGetTime();
	m_lNextGameTick = m_lStartTime;
}
GameEngine::~GameEngine() {}
BOOL GameEngine::CreateGameWindow()
{
	WNDCLASSEX wcApp;
	HWND hWnd = NULL;
	int nPosX, nPosY, nWndWidth, nWndHeight;
	DWORD dwWinStyle;
	wcApp.cbSize = sizeof(wcApp);
	wcApp.lpszClassName = m_szWindowClass;
	wcApp.style = CS_HREDRAW;
	wcApp.lpfnWndProc = WndProc;
	wcApp.hInstance = m_hInstance;
	wcApp.cbWndExtra = 0;
	wcApp.cbClsExtra = 0;
	wcApp.hIconSm = LoadIcon(m_hInstance, MAKEINTRESOURCE(m_wSamllIcon));
	wcApp.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(m_wIcon));
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcApp.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcApp.lpszMenuName = NULL;
	RegisterClassEx(&wcApp);

	DEVMODE DevMode;
	ZeroMemory(&DevMode, sizeof(DevMode));
	DevMode.dmSize = sizeof(DevMode);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode);
	BOOL bDisplayChange = FALSE;
	if (DevMode.dmBitsPerPel != (unsigned long)m_nColorbit)
	{
		DevMode.dmBitsPerPel = m_nColorbit;
		bDisplayChange = TRUE;
	}
	if ((m_bFullScreen && m_nWidth != GetSystemMetrics(SM_CXSCREEN))
		|| m_nWidth > GetSystemMetrics(SM_CXSCREEN))
	{
		DevMode.dmPelsWidth = m_nWidth;
		DevMode.dmPelsHeight = m_nHeight;
		bDisplayChange = TRUE;
	}
	if (bDisplayChange)
	{
		LONG result = ChangeDisplaySettings(&DevMode, 0);
		if (result == DISP_CHANGE_SUCCESSFUL)
		{
			ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
		}
		else
		{
			ChangeDisplaySettings(NULL, 0);
			return FALSE;
		}

	}
	if (m_bFullScreen)
	{
		nPosX = 0;
		nPosY = 0;
		dwWinStyle = WS_POPUP;
		nWndWidth = m_nWidth;
		nWndHeight = m_nHeight;
	}
	else
	{
		nWndWidth = m_nWidth + GetSystemMetrics(SM_CXFIXEDFRAME) * 2;
		nWndHeight = m_nHeight + GetSystemMetrics(SM_CXFIXEDFRAME) * 10;
		nPosX = (GetSystemMetrics(SM_CXSCREEN) - nWndWidth) / 2;
		nPosY = (GetSystemMetrics(SM_CYSCREEN) - nWndHeight) / 2;
		dwWinStyle = WS_SYSMENU | WS_CAPTION;
	}
	hWnd = CreateWindow(m_szWindowClass,
		m_szTitle,
		dwWinStyle,
		nPosX,
		nPosY,
		nWndWidth,
		nWndHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL);
	if (!hWnd)
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdline,
	int nCmdShow)
{
	MSG msg;
	if (!GameInitialize(hInstance))
	{
		return false;
	}
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!GameEngine::GetGameEngine()->GetPause())
			{
				long nTick = timeGetTime();
				long nNextTick = GameEngine::GetGameEngine()->GetNextGameTick();
				if (nTick > nNextTick)
				{
					long nNextGameTick = nTick + GameEngine::GetGameEngine()->GetFrameDelay();
					GameEngine::GetGameEngine()->SetNextGameTick(nNextGameTick);
					GameAction(GameEngine::GetGameEngine()->GetWindow());
				}
			}
		}
	}
	return true;
}
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		GameEngine::GetGameEngine()->SetWindow(hWnd);
		GameStart(hWnd);
		break;
	case WM_PAINT:
	{
		HDC hDC;
		HDC hMemDC;
		HBITMAP hbmMem, hbmOld;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hDC);
		hbmMem = CreateCompatibleBitmap(
			hDC, 
			GameEngine::GetGameEngine()->GetWidth(),
			GameEngine::GetGameEngine()->GetHeight());
		hbmOld = (HBITMAP)SelectObject(hMemDC, hbmMem);
		GamePaint(hMemDC);
		BitBlt(
			hDC,
			0,
			0,
			GameEngine::GetGameEngine()->GetWidth(),
			GameEngine::GetGameEngine()->GetHeight(),
			hMemDC,
			0,
			0,
			SRCCOPY);
		SelectObject(hMemDC, hbmOld);
		DeleteObject(hbmMem);
		EndPaint(hWnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
		MouseClick(hWnd, LOWORD(lParam), HIWORD(lParam), wParam);
		break;
	case WM_SETFOCUS:
		GameActive(hWnd);
		GameEngine::GetGameEngine()->SetPause(FALSE);
		break;
	case WM_KILLFOCUS:
		GamePause(hWnd);
		GameEngine::GetGameEngine()->SetPause(TRUE);
		break;
	case WM_CLOSE:
		if (GameWindowClose(hWnd))
		{
			DestroyWindow(hWnd);
		}
		break;
	case WM_ERASEBKGND:
		return true;
	case WM_DESTROY:
		GameEnd();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return false;
}