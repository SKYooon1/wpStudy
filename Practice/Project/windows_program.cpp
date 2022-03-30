#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_5";

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wndClass;
	g_hInst = hInstance;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = (WNDPROC)wndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = lpszClass;
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(
		lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0,
		800, 600,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	static SIZE size;
	static TCHAR str[10][30];
	static int count[10]{};
	static int yLin{};


	// 메시지 처리하기
	switch (iMessage)
	{										// 메시지 번호
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 10; ++i)
		{
			TextOut(hDC, 0, 0 + (i * 20), str[i], lstrlen(str[i]));
		}
		GetTextExtentPoint32(hDC, str[yLin], lstrlen(str[yLin]), &size);
		SetCaretPos(size.cx, 0 + (20 * yLin));
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_LEFT)
		{
			if (count[yLin] > 0)
				--count[yLin];
		}
		break;
	case WM_CHAR:							// 메시지에 따라 처리
		hDC = GetDC(hWnd);
		if (wParam == VK_BACK)
		{
			if (count[yLin] > 0)
				--count[yLin];
			else
			{
				if (yLin != 0)
					--yLin;
			}
		}
		else if (wParam == VK_RETURN)
		{
			str[yLin][count[yLin]] = '\0';
			++yLin;
			if (yLin == 10)
			{
				yLin = 0;
				for (int i = 0; i < 10; ++i)
					count[i] = 0;
			}
		}
		else if (wParam == VK_ESCAPE)
		{
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 30; ++j)
					str[i][j] = 0;
				count[i] = 0;
			}
			yLin = 0;
		}
		else if (wParam == VK_TAB)
		{
			for (int i = 0; i < 4; ++i)
				str[yLin][count[yLin]++] = ' ';
			str[yLin][count[yLin]] = '\0';
		}
		else
			str[yLin][count[yLin]++] = wParam;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:						// 메시지에 따라 처리
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}										// 처리할 메시지만 case문에 나열
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
