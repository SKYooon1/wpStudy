#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_8";

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

	static RECT rRec{ 280,200,500,350 };
	static RECT rTri{ 30,200,250,350 };
	static RECT rHrg{ 530,200,750,350 };
	static RECT rPen{ 280, 20,500,170 };
	static RECT rPie{ 280,380,500,530 };
	static int rgbRec[6]{}, rgbTri[6]{ 255,0,0 }, rgbHrg[6]{ 0,255,0 },
		rgbPen[6]{ 0,0,255 }, rgbPie[6]{ 255,255,0 };

	RECT temp;
	POINT pTri[3]{ {(rTri.left / 2) + (rTri.right / 2),rTri.top},
		{rTri.left,rTri.bottom},{rTri.right,rTri.bottom} };
	POINT pHrg[6]{ {rHrg.left,rHrg.top},
		{rHrg.left + ((rHrg.right - rHrg.left) / 2),
		rHrg.top + ((rHrg.bottom - rHrg.top) / 2)},
		{ rHrg.left,rHrg.bottom },{ rHrg.right,rHrg.bottom },
		{rHrg.left + ((rHrg.right - rHrg.left) / 2),
		rHrg.top + ((rHrg.bottom - rHrg.top) / 2)},
		{rHrg.right,rHrg.top} };
	POINT pPen[5]{ {rPen.left + ((rPen.right - rPen.left) / 2),rPen.top},
		{rPen.left,rPen.top + ((rPen.bottom - rPen.top) * 2 / 5)},
		{rPen.left + ((rPen.right - rPen.left) / 5),rPen.bottom},
		{rPen.left + ((rPen.right - rPen.left) * 4 / 5),rPen.bottom},
		{rPen.right,rPen.top + ((rPen.bottom - rPen.top) * 2 / 5)} };

	srand((unsigned int)time(0));

	// 메시지 처리하기
	switch (iMessage)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(rgbRec[0], rgbRec[1], rgbRec[2]));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Rectangle(hDC, rRec.left, rRec.top, rRec.right, rRec.bottom);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(rgbTri[0], rgbTri[1], rgbTri[2]));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Polygon(hDC, pTri, 3);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(rgbHrg[0], rgbHrg[1], rgbHrg[2]));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Polygon(hDC, pHrg, 6);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(rgbPen[0], rgbPen[1], rgbPen[2]));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Polygon(hDC, pPen, 5);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(rgbPie[0], rgbPie[1], rgbPie[2]));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Pie(hDC, rPie.left, rPie.top, rPie.right, rPie.bottom,
			rPie.left + ((rPie.right - rPie.left) / 2), rPie.top, rPie.right,
			rPie.top + ((rPie.bottom - rPie.top) / 2));
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);

		switch (wParam)
		{
		case 't':
		case 'T':
			rgbTri[3] = rand() % 10 + rand() % 247;
			rgbTri[4] = rand() % 20 + rand() % 237;
			rgbTri[5] = rand() % 30 + rand() % 227;
			hBrush = CreateSolidBrush(RGB(rgbTri[3], rgbTri[4], rgbTri[5]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Polygon(hDC, pTri, 3);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			break;
		case 's':
		case 'S':
			rgbHrg[3] = rand() % 40 + rand() % 217;
			rgbHrg[4] = rand() % 50 + rand() % 207;
			rgbHrg[5] = rand() % 60 + rand() % 197;
			hBrush = CreateSolidBrush(RGB(rgbHrg[3], rgbHrg[4], rgbHrg[5]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Polygon(hDC, pHrg, 6);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			break;
		case 'p':
		case 'P':
			rgbPen[3] = rand() % 70 + rand() % 187;
			rgbPen[4] = rand() % 80 + rand() % 177;
			rgbPen[5] = rand() % 90 + rand() % 167;
			hBrush = CreateSolidBrush(RGB(rgbPen[3], rgbPen[4], rgbPen[5]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Polygon(hDC, pPen, 5);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			break;
		case 'e':
		case 'E':
			rgbPie[3] = rand() % 100 + rand() % 157;
			rgbPie[4] = rand() % 110 + rand() % 147;
			rgbPie[5] = rand() % 120 + rand() % 137;
			hBrush = CreateSolidBrush(RGB(rgbPie[3], rgbPie[4], rgbPie[5]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Pie(hDC, rPie.left, rPie.top, rPie.right, rPie.bottom,
				rPie.left + ((rPie.right - rPie.left) / 2), rPie.top, rPie.right,
				rPie.top + ((rPie.bottom - rPie.top) / 2));
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
			break;
		case 'Q':
		case 'q':
			PostQuitMessage(0);
			break;
		}

		ReleaseDC(hWnd, hDC);
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'r':
		case 'R':
			InvalidateRect(hWnd, &rRec, true);
			break;
		case 't':
		case 'T':
			InvalidateRect(hWnd, &rTri, true);
			break;
		case 's':
		case 'S':
			InvalidateRect(hWnd, &rHrg, true);
			break;
		case 'p':
		case 'P':
			InvalidateRect(hWnd, &rPen, true);
			break;
		case 'e':
		case 'E':
			InvalidateRect(hWnd, &rPie, true);
			break;
		}
		break;
	case WM_CHAR:
		if (wParam == 'C')
		{
			temp = rTri;
			rTri = rPen;
			rPen = rHrg;
			rHrg = rPie;
			rPie = temp;
			rgbTri[0] = rand() % 10 + rand() % 247;
			rgbTri[1] = rand() % 20 + rand() % 237;
			rgbTri[2] = rand() % 30 + rand() % 227;
			rgbHrg[0] = rand() % 40 + rand() % 217;
			rgbHrg[1] = rand() % 50 + rand() % 207;
			rgbHrg[2] = rand() % 60 + rand() % 197;
			rgbPen[0] = rand() % 70 + rand() % 187;
			rgbPen[1] = rand() % 80 + rand() % 177;
			rgbPen[2] = rand() % 90 + rand() % 167;
			rgbPie[0] = rand() % 100 + rand() % 157;
			rgbPie[1] = rand() % 110 + rand() % 147;
			rgbPie[2] = rand() % 120 + rand() % 137;
			InvalidateRect(hWnd, NULL, true);
		}
		else if (wParam == 'c')
		{
			temp = rTri;
			rTri = rPie;
			rPie = rHrg;
			rHrg = rPen;
			rPen = temp;
			rgbTri[0] = rand() % 10 + rand() % 247;
			rgbTri[1] = rand() % 20 + rand() % 237;
			rgbTri[2] = rand() % 30 + rand() % 227;
			rgbHrg[0] = rand() % 40 + rand() % 217;
			rgbHrg[1] = rand() % 50 + rand() % 207;
			rgbHrg[2] = rand() % 60 + rand() % 197;
			rgbPen[0] = rand() % 70 + rand() % 187;
			rgbPen[1] = rand() % 80 + rand() % 177;
			rgbPen[2] = rand() % 90 + rand() % 167;
			rgbPie[0] = rand() % 100 + rand() % 157;
			rgbPie[1] = rand() % 110 + rand() % 147;
			rgbPie[2] = rand() % 120 + rand() % 137;
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
