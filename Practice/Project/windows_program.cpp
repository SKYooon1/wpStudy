#include "windows_program.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG message;
	WNDCLASSEX wndClass;
	globalHInstance = hInstance;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = static_cast<WNDPROC>(wndProc);
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = lpszClass;
	wndClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wndClass);

	HWND hWnd = CreateWindow(
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

	while (GetMessage(&message, nullptr, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	return message.wParam;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hMemDc;
	static HBITMAP hBitmap;
	static RECT rClient{};

	// 메시지 처리하기
	switch (iMessage)
	{	// 메시지 번호
	case WM_CREATE:
		GetClientRect(hWnd, &rClient);
		hBitmap = (HBITMAP)LoadBitmap(globalHInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hMemDc = CreateCompatibleDC(hdc);
		SelectObject(hMemDc, hBitmap);
		BitBlt(hdc, 0, 0, 640, 426, hMemDc, 0, 0, SRCCOPY);

		DeleteDC(hMemDc);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_CHAR:
	case WM_MOUSEMOVE:
	case WM_KEYDOWN:
	case WM_COMMAND:
	case WM_TIMER:
		break;
	case WM_DESTROY:						// 메시지에 따라 처리
		DeleteObject(hBitmap);
		PostQuitMessage(0);
		break;
	default:
		break;
	}										// 처리할 메시지만 case문에 나열
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
	// case에서 정의되지 않은 메시지는 커널잎 처리하도록 메시지 전달
}
