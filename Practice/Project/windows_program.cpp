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

struct Space
{
	RECT rect{};
	int rgb[3]{};
	int loc{};
};

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH hBrush, oldBrush;

	static RECT rClient{};
	static RECT rect[15][15]{};
	static RECT rText{};
	static Space sBlock[20]{};
	static Space sChange[20]{};
	static Space sUser[2]{};
	static int idx[225]{};
	static bool bTurn{};
	static bool err{};
	TCHAR erText[] = _T(
		"해당 방향으로 움직일 수 없습니다. 다시 시도해주세요.");
	TCHAR er1Text[] = _T("1번 유저의 차례입니다. 다시 시도해주세요.");
	TCHAR er2Text[] = _T("2번 유저의 차례입니다. 다시 시도해주세요.");

	srand((unsigned int)time(0));

	// 메시지 처리하기
	switch (iMessage)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hWnd, &rClient);
		rText = {
			rClient.left, rClient.bottom - 20,
			rClient.right, rClient.bottom
		};
		for (int i = 0; i < 15; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				rect[i][j] = {
					j * (rClient.right / 15),
					i * ((rClient.bottom - 20) / 15),
					(j + 1) * (rClient.right / 15),
					(i + 1) * ((rClient.bottom - 20) / 15)
				};
			}
		}

		for (int i = 0; i < 225; ++i)
			idx[i] = i;
		int iTemp, idx1, idx2;
		for (int i = 0; i < 1000; ++i)
		{
			idx1 = rand() % 225;
			idx2 = rand() % 225;

			iTemp = idx[idx1];
			idx[idx1] = idx[idx2];
			idx[idx2] = iTemp;
		}

		for (int i = 0; i < 20; ++i)
		{
			sBlock[i].loc = idx[i];
			sBlock[i].rect = {
				rect[sBlock[i].loc / 15][sBlock[i].loc % 15].left,
				rect[sBlock[i].loc / 15][sBlock[i].loc % 15].top,
				rect[sBlock[i].loc / 15][sBlock[i].loc % 15].right,
				rect[sBlock[i].loc / 15][sBlock[i].loc % 15].bottom };
			sChange[i].loc = idx[i + 20];
			sChange[i].rect = {
				rect[sChange[i].loc / 15][sChange[i].loc % 15].left,
				rect[sChange[i].loc / 15][sChange[i].loc % 15].top,
				rect[sChange[i].loc / 15][sChange[i].loc % 15].right,
				rect[sChange[i].loc / 15][sChange[i].loc % 15].bottom };
			sChange[i].rgb[0] = rand() % (10 * (i + 1)) + rand() % (256 - (10 * (i + 1)));
			sChange[i].rgb[1] = rand() % (11 * (i + 1)) + rand() % (256 - (11 * (i + 1)));
			sChange[i].rgb[2] = rand() % (12 * (i + 1)) + rand() % (256 - (12 * (i + 1)));
		}

		for (int i = 0; i < 2; ++i)
		{
			sUser[i].loc = idx[i + 40];
			sUser[i].rect = {
				rect[sUser[i].loc / 15][sUser[i].loc % 15].left,
				rect[sUser[i].loc / 15][sUser[i].loc % 15].top,
				rect[sUser[i].loc / 15][sUser[i].loc % 15].right,
				rect[sUser[i].loc / 15][sUser[i].loc % 15].bottom };
		}
		for (int i = 0; i < 3; ++i)
			sUser[1].rgb[i] = 255;

		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		for (int i = 0; i < 15; ++i)
		{
			for (int j = 0; j < 15; ++j)
			{
				Rectangle(hDC, rect[i][j].left, rect[i][j].top,
					rect[i][j].right, rect[i][j].bottom);
			}
		}

		for (int i = 0; i < 20; ++i)
		{
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, sBlock[i].rect.left, sBlock[i].rect.top,
				sBlock[i].rect.right, sBlock[i].rect.bottom);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(sChange[i].rgb[0],
				sChange[i].rgb[1], sChange[i].rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Rectangle(hDC, sChange[i].rect.left, sChange[i].rect.top,
				sChange[i].rect.right, sChange[i].rect.bottom);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		for (int i = 0; i < 2; ++i)
		{
			hBrush = CreateSolidBrush(RGB(sUser[i].rgb[0],
				sUser[i].rgb[1], sUser[i].rgb[2]));
			oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			Ellipse(hDC, sUser[i].rect.left,
				sUser[i].rect.top,
				sUser[i].rect.right,
				sUser[i].rect.bottom);
			SelectObject(hDC, oldBrush);
			DeleteObject(hBrush);
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		hDC = GetDC(hWnd);

		switch (wParam)
		{
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			break;
		case 'r':
		case 'R':
			for (int i = 0; i < 225; ++i)
				idx[i] = i;
			int iTemp, idx1, idx2;
			for (int i = 0; i < 1000; ++i)
			{
				idx1 = rand() % 225;
				idx2 = rand() % 225;

				iTemp = idx[idx1];
				idx[idx1] = idx[idx2];
				idx[idx2] = iTemp;
			}

			for (int i = 0; i < 20; ++i)
			{
				sBlock[i].loc = idx[i];
				sBlock[i].rect = {
					rect[sBlock[i].loc / 15][sBlock[i].loc % 15].left,
					rect[sBlock[i].loc / 15][sBlock[i].loc % 15].top,
					rect[sBlock[i].loc / 15][sBlock[i].loc % 15].right,
					rect[sBlock[i].loc / 15][sBlock[i].loc % 15].bottom };
				sChange[i].loc = idx[i + 20];
				sChange[i].rect = {
					rect[sChange[i].loc / 15][sChange[i].loc % 15].left,
					rect[sChange[i].loc / 15][sChange[i].loc % 15].top,
					rect[sChange[i].loc / 15][sChange[i].loc % 15].right,
					rect[sChange[i].loc / 15][sChange[i].loc % 15].bottom };
				sChange[i].rgb[0] = rand() % (10 * (i + 1)) + rand() % (256 - (10 * (i + 1)));
				sChange[i].rgb[1] = rand() % (11 * (i + 1)) + rand() % (256 - (11 * (i + 1)));
				sChange[i].rgb[2] = rand() % (12 * (i + 1)) + rand() % (256 - (12 * (i + 1)));
			}

			for (int i = 0; i < 2; ++i)
			{
				sUser[i].loc = idx[i + 40];
				sUser[i].rect = {
					rect[sUser[i].loc / 15][sUser[i].loc % 15].left,
					rect[sUser[i].loc / 15][sUser[i].loc % 15].top,
					rect[sUser[i].loc / 15][sUser[i].loc % 15].right,
					rect[sUser[i].loc / 15][sUser[i].loc % 15].bottom };
			}
			for (int i = 0; i < 3; ++i)
				sUser[1].rgb[i] = 255;

			InvalidateRect(hWnd, NULL, true);
			break;
		case 'w':
		case 'W':
			if (bTurn == 0)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[0].loc - 15 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[0].loc < 15)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[0].loc - 15 == sUser[1].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[0].loc - 15 == sChange[i].loc)
						{
							sUser[0].rgb[0] = sChange[i].rgb[0];
							sUser[0].rgb[1] = sChange[i].rgb[1];
							sUser[0].rgb[2] = sChange[i].rgb[2];
						}
					sUser[0].loc -= 15;
					sUser[0].rect = rect[sUser[0].loc / 15][sUser[0].loc % 15];
					bTurn = 1;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er2Text, _tcslen(er2Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 's':
		case 'S':
			if (bTurn == 0)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[0].loc + 15 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[0].loc > 209)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[0].loc + 15 == sUser[1].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[0].loc + 15 == sChange[i].loc)
						{
							sUser[0].rgb[0] = sChange[i].rgb[0];
							sUser[0].rgb[1] = sChange[i].rgb[1];
							sUser[0].rgb[2] = sChange[i].rgb[2];
						}
					sUser[0].loc += 15;
					sUser[0].rect = rect[sUser[0].loc / 15][sUser[0].loc % 15];
					bTurn = 1;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er2Text, _tcslen(er2Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'a':
		case 'A':
			if (bTurn == 0)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[0].loc - 1 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[0].loc % 15 == 0)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[0].loc - 1 == sUser[1].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[0].loc - 1 == sChange[i].loc)
						{
							sUser[0].rgb[0] = sChange[i].rgb[0];
							sUser[0].rgb[1] = sChange[i].rgb[1];
							sUser[0].rgb[2] = sChange[i].rgb[2];
						}
					--sUser[0].loc;
					sUser[0].rect = rect[sUser[0].loc / 15][sUser[0].loc % 15];
					bTurn = 1;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er2Text, _tcslen(er2Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'd':
		case 'D':
			if (bTurn == 0)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[0].loc + 1 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[0].loc % 15 == 14)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[0].loc + 1 == sUser[1].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[0].loc + 1 == sChange[i].loc)
						{
							sUser[0].rgb[0] = sChange[i].rgb[0];
							sUser[0].rgb[1] = sChange[i].rgb[1];
							sUser[0].rgb[2] = sChange[i].rgb[2];
						}
					++sUser[0].loc;
					sUser[0].rect = rect[sUser[0].loc / 15][sUser[0].loc % 15];
					bTurn = 1;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er2Text, _tcslen(er2Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'i':
		case 'I':
			if (bTurn == 1)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[1].loc - 15 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[1].loc < 15)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[1].loc - 15 == sUser[0].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[1].loc - 15 == sChange[i].loc)
						{
							sUser[1].rgb[0] = sChange[i].rgb[0];
							sUser[1].rgb[1] = sChange[i].rgb[1];
							sUser[1].rgb[2] = sChange[i].rgb[2];
						}
					sUser[1].loc -= 15;
					sUser[1].rect = rect[sUser[1].loc / 15][sUser[1].loc % 15];
					bTurn = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er1Text, _tcslen(er1Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'k':
		case 'K':
			if (bTurn == 1)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[1].loc + 15 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[1].loc > 209)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[1].loc + 15 == sUser[0].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[1].loc + 15 == sChange[i].loc)
						{
							sUser[1].rgb[0] = sChange[i].rgb[0];
							sUser[1].rgb[1] = sChange[i].rgb[1];
							sUser[1].rgb[2] = sChange[i].rgb[2];
						}
					sUser[1].loc += 15;
					sUser[1].rect = rect[sUser[1].loc / 15][sUser[1].loc % 15];
					bTurn = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er1Text, _tcslen(er1Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'j':
		case 'J':
			if (bTurn == 1)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[1].loc - 1 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[1].loc % 15 == 0)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[1].loc - 1 == sUser[0].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[1].loc - 1 == sChange[i].loc)
						{
							sUser[1].rgb[0] = sChange[i].rgb[0];
							sUser[1].rgb[1] = sChange[i].rgb[1];
							sUser[1].rgb[2] = sChange[i].rgb[2];
						}
					--sUser[1].loc;
					sUser[1].rect = rect[sUser[1].loc / 15][sUser[1].loc % 15];
					bTurn = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er1Text, _tcslen(er1Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		case 'l':
		case 'L':
			if (bTurn == 1)
			{
				for (int i = 0; i < 20; ++i)
					if (sUser[1].loc + 1 == sBlock[i].loc)
					{
						DrawText(hDC, erText, _tcslen(erText), &rText,
							DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						++err;
						break;
					}
				if (err == 1)
					err = 0;
				else if (sUser[1].loc % 15 == 14)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else if (sUser[1].loc + 1 == sUser[0].loc)
					DrawText(hDC, erText, _tcslen(erText), &rText,
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				else
				{
					for (int i = 0; i < 20; ++i)
						if (sUser[1].loc + 1 == sChange[i].loc)
						{
							sUser[1].rgb[0] = sChange[i].rgb[0];
							sUser[1].rgb[1] = sChange[i].rgb[1];
							sUser[1].rgb[2] = sChange[i].rgb[2];
						}
					++sUser[1].loc;
					sUser[1].rect = rect[sUser[1].loc / 15][sUser[1].loc % 15];
					bTurn = 0;
					InvalidateRect(hWnd, NULL, true);
				}
			}
			else
				DrawText(hDC, er1Text, _tcslen(er1Text), &rText,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			break;
		}

		ReleaseDC(hWnd, hDC);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
