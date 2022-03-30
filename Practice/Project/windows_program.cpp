#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_3";

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
	HBRUSH hBrush, oldBrush;
	RECT rect = { 0, 0, 800, 600 };
	POINT point7[14]{};
	POINT point5[10]{};
	POINT point4[8]{};

	srand((unsigned int)time(0));
	int iSize, iShape;
	iSize = rand() % 9 + 2;

	switch (iMessage) {
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i < iSize; ++i)
		{
			for (int j = 0; j < iSize; ++j)
			{
				iShape = rand() % 6;
				rect.left = i * 800 / iSize;
				rect.top = j * 600 / iSize;
				rect.right = (i + 1) * 800 / iSize;
				rect.bottom = (j + 1) * 600 / iSize;

				switch (iShape)
				{
				case 0:
					MoveToEx(hDC, rect.left, rect.top, NULL);
					LineTo(hDC, rect.right, rect.bottom);

					MoveToEx(hDC, rect.right, rect.top, NULL);
					LineTo(hDC, rect.left, rect.bottom);
					break;
				case 1:
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					point7[0] = { rect.left, rect.top };
					point7[1] = { rect.right, rect.top };
					point7[2] = { rect.left / 2 + rect.right / 2, rect.top / 2 + rect.bottom / 2 };
					point7[3] = { rect.right, rect.bottom };
					point7[4] = { rect.left, rect.bottom };
					point7[5] = { rect.left / 2 + rect.right / 2, rect.top / 2 + rect.bottom / 2 };
					point7[6] = { rect.left, rect.top };

					Polygon(hDC, point7, 7);

					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
					break;
				case 2:
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					point7[0] = { rect.left, rect.top };
					point7[1] = { rect.left, rect.bottom };
					point7[2] = { rect.left / 2 + rect.right / 2, rect.top / 2 + rect.bottom / 2 };
					point7[3] = { rect.right, rect.bottom };
					point7[4] = { rect.right, rect.top };
					point7[5] = { rect.left / 2 + rect.right / 2, rect.top / 2 + rect.bottom / 2 };
					point7[6] = { rect.left, rect.top };

					Polygon(hDC, point7, 7);
					
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
					break;
				case 3:
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					point5[0] = { rect.left / 2 + rect.right / 2, rect.top };
					point5[1] = { rect.right, rect.top / 2 + rect.bottom / 2 };
					point5[2] = { rect.left / 2 + rect.right / 2, rect.bottom };
					point5[3] = { rect.left, rect.top / 2 + rect.bottom / 2 };
					point5[4] = { rect.left / 2 + rect.right / 2, rect.top };

					Polygon(hDC, point5, 5);

					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
					break;
				case 4:
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					point4[0] = { rect.left, rect.top };
					point4[1] = { rect.right, rect.top};
					point4[2] = { rect.left / 2 + rect.right / 2, rect.bottom };
					point4[3] = { rect.left, rect.top };

					Polygon(hDC, point4, 4);

					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
					break;
				case 5:
					hBrush = CreateSolidBrush(RGB(0, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);

					FillRect(hDC, &rect, hBrush);

					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
					break;
				}
			}
		}
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
