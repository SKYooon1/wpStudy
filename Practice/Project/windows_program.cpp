#include <Windows.h>
#include <tchar.h>
#include <random>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 3_2";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX wndClass;
	g_hInst = hInstance;

	wndClass.cbSize = sizeof(wndClass);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

struct Block
{
	RECT rect;
	int stat;
};

BOOL InBar(int x, int y, RECT rect)
{
	if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom)
		return true;
	else false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, oldBrush;

	static RECT rClient{};
	static RECT rBar{};
	static RECT rBall{};
	static Block bBlock[40]{};
	static int iSpeed{};
	static int xSpeed{};
	static int ySpeed{};
	static int xMouse{};
	static int yMouse{};
	static int iStep{};
	static int Line[4]{};
	static bool Selection{};
	static bool Move{};
	static bool bShape{};
	static bool bBmove{};
	static char cColor{};

	// 메시지 처리하기
	switch (uMsg)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hwnd, &rClient);

		Line[0] = false;
		Line[1] = false;
		Move = false;
		iSpeed = 30;
		xSpeed = 0;
		ySpeed = -iSpeed;
		cColor = 'y';
		bShape = false;
		xMouse = rClient.left + 400;
		iStep = 3;
		bBmove = false;

		for (int i = 0; i < iStep; ++i)
			for (int j = 0; j < 10; ++j)
			{
				bBlock[(i * 10) + j].rect.left = rClient.right * (j + 1) / 12;
				bBlock[(i * 10) + j].rect.right = rClient.right * (j + 2) / 12;
				bBlock[(i * 10) + j].rect.top = rClient.top + (50 * i);
				bBlock[(i * 10) + j].rect.bottom = rClient.top + (50 * (i + 1));
				bBlock[(i * 10) + j].stat = 0;
			}

		rBar = {
			{rClient.left + 300},
			{rClient.bottom - 75},
			{rClient.left + 500},
			{rClient.bottom - 25}
		};

		rBall = {
			{rClient.left + 385},
			{rClient.bottom - 106},
			{rClient.left + 415},
			{rClient.bottom - 75}
		};

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < iStep; ++i)
		{
			switch (Line[i])
			{
			case 0:
				hBrush = CreateSolidBrush(RGB(255, 255, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				for (int j = 0; j < 10; ++j)
				{
					Rectangle(hdc, bBlock[(i * 10) + j].rect.left,
						bBlock[(i * 10) + j].rect.top,
						bBlock[(i * 10) + j].rect.right,
						bBlock[(i * 10) + j].rect.bottom);
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			case 1:
				hBrush = CreateSolidBrush(RGB(0, 0, 255));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				for (int j = 0; j < 10; ++j)
				{
					Rectangle(hdc, bBlock[(i * 10) + j].rect.left,
						bBlock[(i * 10) + j].rect.top,
						bBlock[(i * 10) + j].rect.right,
						bBlock[(i * 10) + j].rect.bottom);
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			case 2:
				hBrush = CreateSolidBrush(RGB(255, 0, 255));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				for (int j = 0; j < 10; ++j)
				{
					Rectangle(hdc, bBlock[(i * 10) + j].rect.left,
						bBlock[(i * 10) + j].rect.top,
						bBlock[(i * 10) + j].rect.right,
						bBlock[(i * 10) + j].rect.bottom);
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			case 3:
				hBrush = CreateSolidBrush(RGB(0, 255, 0));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				for (int j = 0; j < 10; ++j)
				{
					Rectangle(hdc, bBlock[(i * 10) + j].rect.left,
						bBlock[(i * 10) + j].rect.top,
						bBlock[(i * 10) + j].rect.right,
						bBlock[(i * 10) + j].rect.bottom);
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			case 4:
				hBrush = CreateSolidBrush(RGB(0, 255, 255));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				for (int j = 0; j < 10; ++j)
				{
					Rectangle(hdc, bBlock[(i * 10) + j].rect.left,
						bBlock[(i * 10) + j].rect.top,
						bBlock[(i * 10) + j].rect.right,
						bBlock[(i * 10) + j].rect.bottom);
				}
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			}
		}

		switch (cColor)
		{
		case 'c':
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rBar.left, rBar.top, rBar.right, rBar.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		case 'm':
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rBar.left, rBar.top, rBar.right, rBar.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		case 'y':
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rBar.left, rBar.top, rBar.right, rBar.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			break;
		}

		if (bShape)
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, rBall.left, rBall.top, rBall.right, rBall.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, rBall.left, rBall.top, rBall.right, rBall.bottom);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			if (Move)
			{
				rBall.left += xSpeed;
				rBall.right += xSpeed;
				rBall.top += ySpeed;
				rBall.bottom += ySpeed;
				InvalidateRect(hwnd, &rClient, true);
			}
			if (rBall.bottom >= rBar.top && !(rBall.top > rBar.top)
				&& ((rBall.left + rBall.right) / 2) >= rBar.left
				&& ((rBall.left + rBall.right) / 2) <= rBar.right)
			{
				if (xSpeed < 0)
				{
					xSpeed = -iSpeed / 2;
					ySpeed = -iSpeed / 2;
				}
				else
				{
					xSpeed = iSpeed / 2;
					ySpeed = -iSpeed / 2;
				}
			}
			if (rBall.right >= rClient.right || rBall.left <= rClient.left)
				xSpeed = -xSpeed;
			if (rBall.top <= rClient.top)
				ySpeed = -ySpeed;
			if (rBall.top >= rClient.bottom)
			{
				Move = false;
				rBall = {
					{xMouse - 15},
					{rClient.bottom - 106},
					{xMouse + 15},
					{rClient.bottom - 75}
				};
			}
			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					if (i == 0 && rBall.top < bBlock[j].rect.bottom
						&& ((rBall.left + rBall.right) / 2 >= bBlock[j].rect.left)
						&& ((rBall.left + rBall.right) / 2 <= bBlock[j].rect.right))
					{
						Line[i] = i + 1;
						if (bBlock[j].stat == 1)
						{
							bBlock[j].rect.left = 0;
							bBlock[j].rect.top = 0;
							bBlock[j].rect.right = 0;
							bBlock[j].rect.bottom = 0;
							bBlock[j].stat++;
						}

						if ((rBall.left + rBall.right) / 2
							== (bBlock[j].rect.left + bBlock[j].rect.right) / 2)
						{
							xSpeed = 0;
							ySpeed = iSpeed;
							bBlock[j].stat++;
							bBlock[j].rect.top += 50;
							bBlock[j].rect.bottom += 50;
						}
						else if ((rBall.left + rBall.right) / 2
							< (bBlock[j].rect.left + bBlock[j].rect.right) / 2)
						{
							xSpeed = -iSpeed / 2;
							ySpeed = iSpeed / 2;
							bBlock[j].stat++;
							bBlock[j].rect.top += 50;
							bBlock[j].rect.bottom += 50;
						}
						else
						{
							xSpeed = iSpeed / 2;
							ySpeed = iSpeed / 2;
							bBlock[j].stat++;
							bBlock[j].rect.top += 50;
							bBlock[j].rect.bottom += 50;
						}
						break;
					}
					else if (rBall.top < bBlock[(i * 10) + j].rect.bottom
						&& ((rBall.left + rBall.right) / 2
							>= bBlock[(i * 10) + j].rect.left)
						&& ((rBall.left + rBall.right) / 2
							<= bBlock[(i * 10) + j].rect.right)
						&& ((rBall.top + rBall.bottom) / 2
					> bBlock[((i - 1) * 10) + j].rect.bottom))
					{
						Line[i] = i + 1;

						if (bBlock[(i * 10) + j].stat == 1)
						{
							bBlock[(i * 10) + j].rect.left = 0;
							bBlock[(i * 10) + j].rect.top = 0;
							bBlock[(i * 10) + j].rect.right = 0;
							bBlock[(i * 10) + j].rect.bottom = 0;
							bBlock[(i * 10) + j].stat++;
						}

						if ((rBall.left + rBall.right) / 2
							== (bBlock[(i * 10) + j].rect.left
								+ bBlock[(i * 10) + j].rect.right) / 2)
						{
							xSpeed = 0;
							ySpeed = iSpeed;
							bBlock[(i * 10) + j].stat++;
							bBlock[(i * 10) + j].rect.top += 50;
							bBlock[(i * 10) + j].rect.bottom += 50;
						}
						else if ((rBall.left + rBall.right) / 2
							< (bBlock[(i * 10) + j].rect.left
								+ bBlock[(i * 10) + j].rect.right) / 2)
						{
							xSpeed = -iSpeed / 2;
							ySpeed = iSpeed / 2;
							bBlock[(i * 10) + j].stat++;
							bBlock[(i * 10) + j].rect.top += 50;
							bBlock[(i * 10) + j].rect.bottom += 50;
						}
						else
						{
							xSpeed = iSpeed / 2;
							ySpeed = iSpeed / 2;
							bBlock[(i * 10) + j].stat++;
							bBlock[(i * 10) + j].rect.top += 50;
							bBlock[(i * 10) + j].rect.bottom += 50;
						}
						break;
					}
				}
			break;
		case 2:
			if (bBmove)
			{
				for (int i = 0; i < iStep; ++i)
					for (int j = 0; j < 10; ++j)
					{
						if (bBlock[(i * 10) + 9].rect.right >= rClient.right - 10)
						{
							bBlock[(i * 10) + j].rect.left
								-= rClient.right * 2 / 12;
							bBlock[(i * 10) + j].rect.right
								-= rClient.right * 2 / 12;
						}
						else
						{
							bBlock[(i * 10) + j].rect.left
								+= rClient.right / 12;
							bBlock[(i * 10) + j].rect.right
								+= rClient.right / 12;
						}
					}
			}
			InvalidateRect(hwnd, &rClient, true);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (InBar(LOWORD(lParam), HIWORD(lParam), rBar) && Move)
		{
			Selection = true;
			rBar = {
			{LOWORD(lParam) - 100},
			{rClient.bottom - 75},
			{LOWORD(lParam) + 100},
			{rClient.bottom - 25}
			};
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		Selection = false;
		break;
	case WM_MOUSEMOVE:
		if (Selection && Move)
		{
			rBar = {
				{LOWORD(lParam) - 100},
				{rClient.bottom - 75},
				{LOWORD(lParam) + 100},
				{rClient.bottom - 25}
			};
			xMouse = LOWORD(lParam);
			yMouse = HIWORD(lParam);
		}
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			break;
		case 's':
		case 'S':
			Move = true;
			InvalidateRect(hwnd, &rClient, true);
			SetTimer(hwnd, 1, 100, NULL);
			break;
		case 'p':
		case 'P':
			Move = false;
			InvalidateRect(hwnd, &rClient, true);
			break;
		case '+':
			if (iSpeed < 50)
			{
				iSpeed += 10;
				if (xSpeed == 0)
				{
					if (ySpeed > 0)
						ySpeed = iSpeed;
					else
						ySpeed = -iSpeed;
				}
				else
				{
					if (xSpeed > 0)
						xSpeed = iSpeed / 2;
					else
						xSpeed = -iSpeed / 2;
					if (ySpeed > 0)
						ySpeed = iSpeed / 2;
					else
						ySpeed = -iSpeed / 2;
				}
			}
			break;
		case '-':
			if (iSpeed > 10)
				iSpeed -= 10;
			if (xSpeed == 0)
			{
				if (ySpeed > 0)
					ySpeed = iSpeed;
				else
					ySpeed = -iSpeed;
			}
			else
			{
				if (xSpeed > 0)
					xSpeed = iSpeed / 2;
				else
					xSpeed = -iSpeed / 2;
				if (ySpeed > 0)
					ySpeed = iSpeed / 2;
				else
					ySpeed = -iSpeed / 2;
			}
			break;
		case 'n':
		case 'N':
			Line[0] = false;
			Line[1] = false;
			Move = false;
			iSpeed = 30;
			xSpeed = 0;
			ySpeed = -iSpeed;
			cColor = 'y';
			bShape = false;
			xMouse = rClient.left + 400;
			iStep = 2;
			bBmove = false;

			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					bBlock[(i * 10) + j].rect.left = rClient.right * (j + 1) / 12;
					bBlock[(i * 10) + j].rect.right = rClient.right * (j + 2) / 12;
					bBlock[(i * 10) + j].rect.top = rClient.top + (50 * i);
					bBlock[(i * 10) + j].rect.bottom = rClient.top + (50 * (i + 1));
					bBlock[(i * 10) + j].stat = 0;
				}

			rBar = {
				{rClient.left + 300},
				{rClient.bottom - 75},
				{rClient.left + 500},
				{rClient.bottom - 25}
			};

			rBall = {
				{rClient.left + 385},
				{rClient.bottom - 106},
				{rClient.left + 415},
				{rClient.bottom - 75}
			};
			InvalidateRect(hwnd, &rClient, true);
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_GM_ST:
			Move = true;
			InvalidateRect(hwnd, &rClient, true);
			SetTimer(hwnd, 1, 100, NULL);
			break;
		case ID_GM_RS:
			Line[0] = false;
			Line[1] = false;
			Move = false;
			iSpeed = 30;
			xSpeed = 0;
			ySpeed = -iSpeed;
			cColor = 'y';
			bShape = false;
			xMouse = rClient.left + 400;
			iStep = 2;
			bBmove = false;

			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					bBlock[(i * 10) + j].rect.left = rClient.right * (j + 1) / 12;
					bBlock[(i * 10) + j].rect.right = rClient.right * (j + 2) / 12;
					bBlock[(i * 10) + j].rect.top = rClient.top + (50 * i);
					bBlock[(i * 10) + j].rect.bottom = rClient.top + (50 * (i + 1));
					bBlock[(i * 10) + j].stat = 0;
				}

			rBar = {
				{rClient.left + 300},
				{rClient.bottom - 75},
				{rClient.left + 500},
				{rClient.bottom - 25}
			};

			rBall = {
				{rClient.left + 385},
				{rClient.bottom - 106},
				{rClient.left + 415},
				{rClient.bottom - 75}
			};
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_GM_ED:
			PostQuitMessage(0);
			break;
		case ID_SP_FA:
			iSpeed = 50;
			if (xSpeed == 0)
			{
				if (ySpeed > 0)
					ySpeed = iSpeed;
				else
					ySpeed = -iSpeed;
			}
			else
			{
				if (xSpeed > 0)
					xSpeed = iSpeed / 2;
				else
					xSpeed = -iSpeed / 2;
				if (ySpeed > 0)
					ySpeed = iSpeed / 2;
				else
					ySpeed = -iSpeed / 2;
			}
			break;
		case ID_SP_ME:
			iSpeed = 30;
			if (xSpeed == 0)
			{
				if (ySpeed > 0)
					ySpeed = iSpeed;
				else
					ySpeed = -iSpeed;
			}
			else
			{
				if (xSpeed > 0)
					xSpeed = iSpeed / 2;
				else
					xSpeed = -iSpeed / 2;
				if (ySpeed > 0)
					ySpeed = iSpeed / 2;
				else
					ySpeed = -iSpeed / 2;
			}
			break;
		case ID_SP_SL:
			iSpeed = 10;
			if (xSpeed == 0)
			{
				if (ySpeed > 0)
					ySpeed = iSpeed;
				else
					ySpeed = -iSpeed;
			}
			else
			{
				if (xSpeed > 0)
					xSpeed = iSpeed / 2;
				else
					xSpeed = -iSpeed / 2;
				if (ySpeed > 0)
					ySpeed = iSpeed / 2;
				else
					ySpeed = -iSpeed / 2;
			}
			break;
		case ID_CO_CY:
			cColor = 'c';
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_CO_MA:
			cColor = 'm';
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_CO_YE:
			cColor = 'y';
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_SH_CI:
			bShape = false;
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_SH_RE:
			bShape = true;
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_ST_2:
			iStep = 2;
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_ST_3:
			iStep = 3;
			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					bBlock[(i * 10) + j].rect.left = rClient.right * (j + 1) / 12;
					bBlock[(i * 10) + j].rect.right = rClient.right * (j + 2) / 12;
					bBlock[(i * 10) + j].rect.top = rClient.top + (50 * i);
					bBlock[(i * 10) + j].rect.bottom = rClient.top + (50 * (i + 1));
					bBlock[(i * 10) + j].stat = 0;
				}
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_ST_4:
			iStep = 4;
			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					bBlock[(i * 10) + j].rect.left = rClient.right * (j + 1) / 12;
					bBlock[(i * 10) + j].rect.right = rClient.right * (j + 2) / 12;
					bBlock[(i * 10) + j].rect.top = rClient.top + (50 * i);
					bBlock[(i * 10) + j].rect.bottom = rClient.top + (50 * (i + 1));
					bBlock[(i * 10) + j].stat = 0;
				}
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_MV_ON:
			bBmove = true;
			for (int i = 0; i < iStep; ++i)
				for (int j = 0; j < 10; ++j)
				{
					if (bBlock[(i * 10) + j].rect.right >= rClient.right)
					{
						bBlock[(i * 10) + j].rect.left
							-= rClient.right * 2 / 12;
						bBlock[(i * 10) + j].rect.right
							-= rClient.right * 2 / 12;
					}
					else
					{
						bBlock[(i * 10) + j].rect.left
							+= rClient.right / 12;
						bBlock[(i * 10) + j].rect.right
							+= rClient.right / 12;
					}
				}
			SetTimer(hwnd, 2, 1000, NULL);
			InvalidateRect(hwnd, &rClient, true);
			break;
		case ID_MV_OF:
			bBmove = false;
			break;
		}
		break;
	case WM_DESTROY:						// 메시지에 따라 처리
		PostQuitMessage(0);
		break;
	}										// 처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	// case에서 정의되지 않은 메시지는 커널잎 처리하도록 메시지 전달
}
