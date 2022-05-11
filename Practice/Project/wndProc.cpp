#include "wndProc.h"

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps{};
	HDC hdc{}, memdc{};
	static HBITMAP hBitmap;

	static RECT rClient{};
	static int pc{};
	static bool selection{};

	std::string strNum{};
	std::vector<int> vecNum{};

	// 메시지 처리하기
	switch (iMessage)
	{	// 메시지 번호
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(globalHInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		GetClientRect(hWnd, &rClient);
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		BitBlt(hdc, 0, 0, 800, 500, memdc, 0, 0, SRCCOPY);
		DeleteDC(memdc);
		
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
