#include <windows.h>
#include <tchar.h>
#include <random>

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
	static Block bBlock[30]{};
	static int iSpeed{};
	static int xSpeed{};
	static int ySpeed{};
	static bool Selection{};
	static bool Line[3]{};
	static bool Move{};

	// 메시지 처리하기
	switch (uMsg)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hwnd, &rClient);

		for (int i = 0; i < 10; ++i)
		{
			bBlock[i].rect.left = rClient.right * (i + 1) / 12;
			bBlock[i].rect.right = rClient.right * (i + 2) / 12;
			bBlock[i].rect.top = rClient.top;
			bBlock[i].rect.bottom = rClient.top + 50;
			bBlock[i + 10].rect.left = rClient.right * (i + 1) / 12;
			bBlock[i + 10].rect.right = rClient.right * (i + 2) / 12;
			bBlock[i + 10].rect.top = rClient.top + 50;
			bBlock[i + 10].rect.bottom = rClient.top + 100;
			bBlock[i + 20].rect.left = rClient.right * (i + 1) / 12;
			bBlock[i + 20].rect.right = rClient.right * (i + 2) / 12;
			bBlock[i + 20].rect.top = rClient.top + 100;
			bBlock[i + 20].rect.bottom = rClient.top + 150;
			bBlock[i].stat = 0;
			bBlock[i + 10].stat = 0;
			bBlock[i + 20].stat = 0;
		}

		Line[0] = false;
		Line[1] = false;
		Line[2] = false;
		Move = false;
		iSpeed = 30;

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

		if (Line[0] == false)
		{
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i].rect.left, bBlock[i].rect.top,
					bBlock[i].rect.right, bBlock[i].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i].rect.left, bBlock[i].rect.top,
					bBlock[i].rect.right, bBlock[i].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		if (Line[1] == false)
		{
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i + 10].rect.left, bBlock[i + 10].rect.top,
					bBlock[i + 10].rect.right, bBlock[i + 10].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i + 10].rect.left, bBlock[i + 10].rect.top,
					bBlock[i + 10].rect.right, bBlock[i + 10].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		if (Line[2] == false)
		{
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i + 20].rect.left, bBlock[i + 20].rect.top,
					bBlock[i + 20].rect.right, bBlock[i + 20].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		else
		{
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			for (int i = 0; i < 10; ++i)
			{
				Rectangle(hdc, bBlock[i + 20].rect.left, bBlock[i + 20].rect.top,
					bBlock[i + 20].rect.right, bBlock[i + 20].rect.bottom);
			}
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}

		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, rBar.left, rBar.top, rBar.right, rBar.bottom);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, rBall.left, rBall.top, rBall.right, rBall.bottom);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

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
				&& ((rBall.left + rBall.right) / 2))
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
					{LOWORD(lParam) - 15},
					{rClient.bottom - 106},
					{LOWORD(lParam) + 15},
					{rClient.bottom - 75}
				};
			}
			for (int i = 0; i < 10; ++i)
			{
				if (rBall.top < bBlock[i].rect.bottom
					&& ((rBall.left + rBall.right) / 2 >= bBlock[i].rect.left)
					&& ((rBall.left + rBall.right) / 2 <= bBlock[i].rect.right))
				{
					Line[0] = true;
					if (bBlock[i].stat == 1)
					{
						bBlock[i].rect.left = 0;
						bBlock[i].rect.top = 0;
						bBlock[i].rect.right = 0;
						bBlock[i].rect.bottom = 0;
						bBlock[i].stat++;
					}

					if ((rBall.left + rBall.right) / 2
						== (bBlock[i].rect.left + bBlock[i].rect.right) / 2)
					{
						xSpeed = 0;
						ySpeed = iSpeed;
						bBlock[i].stat++;
						bBlock[i].rect.top += 50;
						bBlock[i].rect.bottom += 50;
					}
					else if ((rBall.left + rBall.right) / 2
						< (bBlock[i].rect.left + bBlock[i].rect.right) / 2)
					{
						xSpeed = -iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i].stat++;
						bBlock[i].rect.top += 50;
						bBlock[i].rect.bottom += 50;
					}
					else
					{
						xSpeed = iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i].stat++;
						bBlock[i].rect.top += 50;
						bBlock[i].rect.bottom += 50;
					}
					break;
				}
				if (rBall.top < bBlock[i + 10].rect.bottom
					&& ((rBall.left + rBall.right) / 2
						>= bBlock[i + 10].rect.left)
					&& ((rBall.left + rBall.right) / 2
						<= bBlock[i + 10].rect.right)
					&& ((rBall.top + rBall.bottom) / 2 > bBlock[i].rect.bottom))
				{
					Line[1] = true;

					if (bBlock[i + 10].stat == 1)
					{
						bBlock[i + 10].rect.left = 0;
						bBlock[i + 10].rect.top = 0;
						bBlock[i + 10].rect.right = 0;
						bBlock[i + 10].rect.bottom = 0;
						bBlock[i + 10].stat++;
					}

					if ((rBall.left + rBall.right) / 2
						== (bBlock[i + 10].rect.left
							+ bBlock[i + 10].rect.right) / 2)
					{
						xSpeed = 0;
						ySpeed = iSpeed;
						bBlock[i + 10].stat++;
						bBlock[i + 10].rect.top += 50;
						bBlock[i + 10].rect.bottom += 50;
					}
					else if ((rBall.left + rBall.right) / 2
						< (bBlock[i + 10].rect.left
							+ bBlock[i + 10].rect.right) / 2)
					{
						xSpeed = -iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i + 10].stat++;
						bBlock[i + 10].rect.top += 50;
						bBlock[i + 10].rect.bottom += 50;
					}
					else
					{
						xSpeed = iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i + 10].stat++;
						bBlock[i + 10].rect.top += 50;
						bBlock[i + 10].rect.bottom += 50;
					}
					break;
				}
				if (rBall.top < bBlock[i + 20].rect.bottom
					&& ((rBall.left + rBall.right) / 2
						>= bBlock[i + 20].rect.left)
					&& ((rBall.left + rBall.right) / 2
						<= bBlock[i + 20].rect.right)
					&& ((rBall.top + rBall.bottom) / 2 > bBlock[i].rect.bottom))
				{
					Line[1] = true;

					if (bBlock[i + 20].stat == 1)
					{
						bBlock[i + 20].rect.left = 0;
						bBlock[i + 20].rect.top = 0;
						bBlock[i + 20].rect.right = 0;
						bBlock[i + 20].rect.bottom = 0;
						bBlock[i + 20].stat++;
					}

					if ((rBall.left + rBall.right) / 2
						== (bBlock[i + 20].rect.left
							+ bBlock[i + 20].rect.right) / 2)
					{
						xSpeed = 0;
						ySpeed = iSpeed;
						bBlock[i + 20].stat++;
						bBlock[i + 20].rect.top += 50;
						bBlock[i + 20].rect.bottom += 50;
					}
					else if ((rBall.left + rBall.right) / 2
						< (bBlock[i + 20].rect.left
							+ bBlock[i + 20].rect.right) / 2)
					{
						xSpeed = -iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i + 20].stat++;
						bBlock[i + 20].rect.top += 50;
						bBlock[i + 20].rect.bottom += 50;
					}
					else
					{
						xSpeed = iSpeed / 2;
						ySpeed = iSpeed / 2;
						bBlock[i + 20].stat++;
						bBlock[i + 20].rect.top += 50;
						bBlock[i + 20].rect.bottom += 50;
					}
					break;
				}
			}
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
			xSpeed = 0;
			ySpeed = -iSpeed;
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
				iSpeed += 10;
			break;
		case '-':
			if (iSpeed > 10)
				iSpeed -= 10;
			break;
		case 'n':
		case 'N':
			for (int i = 0; i < 10; ++i)
			{
				bBlock[i].rect.left = rClient.right * (i + 1) / 12;
				bBlock[i].rect.right = rClient.right * (i + 2) / 12;
				bBlock[i].rect.top = rClient.top;
				bBlock[i].rect.bottom = rClient.top + 50;
				bBlock[i + 10].rect.left = rClient.right * (i + 1) / 12;
				bBlock[i + 10].rect.right = rClient.right * (i + 2) / 12;
				bBlock[i + 10].rect.top = rClient.top + 50;
				bBlock[i + 10].rect.bottom = rClient.top + 100;
				bBlock[i + 20].rect.left = rClient.right * (i + 1) / 12;
				bBlock[i + 20].rect.right = rClient.right * (i + 2) / 12;
				bBlock[i + 20].rect.top = rClient.top + 100;
				bBlock[i + 20].rect.bottom = rClient.top + 150;
				bBlock[i].stat = 0;
				bBlock[i + 10].stat = 0;
				bBlock[i + 20].stat = 0;
			}

			Line[0] = false;
			Line[1] = false;
			Move = false;
			iSpeed = 30;

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
	case WM_DESTROY:						// 메시지에 따라 처리
		PostQuitMessage(0);
		break;
	}										// 처리할 메시지만 case문에 나열
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
	// case에서 정의되지 않은 메시지는 커널잎 처리하도록 메시지 전달
}
