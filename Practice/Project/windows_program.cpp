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
	static TCHAR str[10][80];
	static int count[10]{};
	static int yLin{};
	static bool flagUpper{};

	switch (iMessage)
	{
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
		GetTextExtentPoint32(hDC, str[yLin], count[yLin], &size);
		SetCaretPos(size.cx, 0 + (20 * yLin));
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			if (count[yLin] > 0)
				--count[yLin];
			break;
		case VK_UP:
			if (yLin > 0)
				--yLin;
			break;
		case VK_RIGHT:
			if (count[yLin] < lstrlen(str[yLin]))
				++count[yLin];
			break;
		case VK_DOWN:
			if (yLin < 10)
				++yLin;
			break;
		case VK_F1:
			flagUpper = !flagUpper;
			break;
		case VK_DELETE:
			if (count[yLin] < lstrlen(str[yLin]))
			{
				str[yLin][++count[yLin]] = 0;
			}
			break;
		case VK_HOME:
			count[yLin] = 0;
			break;
		case VK_END:
			while (str[yLin][count[yLin]] != 0)
				++count[yLin];
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		hDC = GetDC(hWnd);
		switch (wParam)
		{
		case VK_BACK:
			if (count[yLin] > 0)
			{
				str[yLin][--count[yLin]] = 0;
			}
			else
			{
				if (yLin != 0)
					--yLin;
			}
			break;
		case VK_RETURN:
			str[yLin][count[yLin]] = '\0';
			++yLin;
			if (yLin == 10)
			{
				yLin = 0;
				for (int i = 0; i < 10; ++i)
					count[i] = 0;
			}
			break;
		case VK_ESCAPE:
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 80; ++j)
					str[i][j] = 0;
				count[i] = 0;
			}
			yLin = 0;
			break;
		case VK_TAB:
			for (int i = 0; i < 4; ++i)
				str[yLin][count[yLin]++] = ' ';
			str[yLin][count[yLin]] = '\0';
			break;
		default:
			if (count[yLin] == 79)
			{
				str[yLin][count[yLin]] = '\0';
				++yLin;
			}
			else if (flagUpper)
			{
				str[yLin][count[yLin]++] = toupper(wParam);
			}
			else
			{
				str[yLin][count[yLin]++] = wParam;
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
