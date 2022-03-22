#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_2";

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
		1280, 800,
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
	RECT rect1{ 0, 0, 640, 400 };
	RECT rect2{ 640, 0, 1280, 400 };
	RECT rect3{ 0, 400, 640, 800 };
	RECT rect4{ 640, 400, 1280, 800 };
	std::default_random_engine dre;
	std::uniform_int_distribution<int> uid;

	switch (iMessage) {
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		SetBkColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		SetTextColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		DrawText(hDC, L"This is left-top area", strlen("This is left-top area"), &rect1, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		SetBkColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		SetTextColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		DrawText(hDC, L"This is right-top area", strlen("This is right-top area"), &rect2, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		SetBkColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		SetTextColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		DrawText(hDC, L"This is left-bottom area", strlen("This is left-bottom area"), &rect3, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		SetBkColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		SetTextColor(hDC, RGB(uid(dre), uid(dre), uid(dre)));
		DrawText(hDC, L"This is right-bottom area", strlen("This is right-bottom area"), &rect4, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
