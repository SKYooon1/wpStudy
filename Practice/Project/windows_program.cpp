#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_3";

std::default_random_engine dre;
std::uniform_int_distribution<int> uid_x{ 0, 700 };
std::uniform_int_distribution<int> uid_y{ 0, 400 };
std::uniform_int_distribution<int> uid_n{ 0, 9 };
std::uniform_int_distribution<int> uid_count{ 5, 10 };
std::uniform_int_distribution<int> uid_color{ 0, 700 };

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

	int x = uid_x(dre);
	int y = uid_y(dre);
	int n = uid_n(dre);
	int count = uid_count(dre);
	int line = 16;

	RECT rect{ x, y, x + 100, y + 100 };

	TCHAR str[101];

	for (int i = 0; i < count; ++i)
	{
		str[i] = n + '0';
	}
	str[count] = '\0';


	switch (iMessage) {
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		SetBkColor(hDC, RGB(uid_color(dre), uid_color(dre), uid_color(dre)));
		SetTextColor(hDC, RGB(uid_color(dre), uid_color(dre), uid_color(dre)));
		
		for (int i = 0; i < count; ++i)
		{
			TextOut(hDC, x, y + line * i, str, lstrlen(str));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
