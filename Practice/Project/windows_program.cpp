#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 3_1";

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

struct Circle
{
	RECT rect;
	int tail;
	int main;
	int sub;
	int loc;
	int sp;
};

float LengthPts(int x1, int y1, int x2, int y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, RECT rect)
{
	if (LengthPts(x, y, (rect.left / 2) + (rect.right / 2),
		(rect.top / 2) + (rect.bottom / 2)) < ((rect.right - rect.left) / 2))
		return true;
	else
		return false;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, oldBrush;

	static RECT rClient{};
	static RECT rBoard[40][40]{};
	static Circle cMain{};
	static Circle cTail[20]{};
	static int iSc{};
	static int iSpeed;
	static bool bStart = false;

	bool bTI = true;

	// 메시지 처리하기
	switch (uMsg)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hwnd, &rClient);

		srand((unsigned int)time(0));

		SetTimer(hwnd, 1, 5000, NULL);

		for (int i = 0; i < 40; ++i)
			for (int j = 0; j < 40; ++j)
				rBoard[i][j] = {
					{rClient.right * j / 40},
					{rClient.bottom * i / 40},
					{rClient.right * (j + 1) / 40},
					{rClient.bottom * (i + 1) / 40}
			};

		cMain.rect = rBoard[0][0];
		cMain.main = 1;
		cMain.sub = 3;
		cMain.tail = 0;
		cMain.loc = 0;
		cMain.sp = 100;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < 40; ++i)
			for (int j = 0; j < 40; ++j)
				Rectangle(hdc, rBoard[i][j].left, rBoard[i][j].top,
					rBoard[i][j].right, rBoard[i][j].bottom);

		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, cMain.rect.left, cMain.rect.top,
			cMain.rect.right, cMain.rect.bottom);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);


		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (int i = 0; i <= iSc; ++i)
		{
			Ellipse(hdc, cTail[i].rect.left, cTail[i].rect.top,
				cTail[i].rect.right, cTail[i].rect.bottom);
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hwnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			for (int i = 0; i < iSc; ++i)
				if (cTail[i].rect.left == rBoard[20][20].left)
					bTI = false;
			if (cMain.rect.left == rBoard[20][20].left)
				bTI = false;
			if (iSc < 20 && bTI == true)
			{
				cTail[iSc].rect = rBoard[20][20];
				cTail[iSc].main = rand() % 4;
				cTail[iSc].tail = -1;
				cTail[iSc].loc = 420;
				iSc++;
			}
			break;
		case 2:
			if (iSpeed == 0)
			{
				cMain.sp += 10;
				SetTimer(hwnd, 2, cMain.sp, NULL);
			}
			for (int i = 0; i < iSc; ++i)
			{
				if (cTail[i].tail == -1)
				{
					switch (cTail[i].main)
					{
					case 0:
						cTail[i].loc--;
						cTail[i].rect = rBoard[cTail[i].loc / 40][cTail[i].loc % 40];
						if (cTail[i].loc % 40 == 0)
							while (cTail[i].main == 0)
								cTail[i].main = rand() % 4;
						break;
					case 1:
						cTail[i].loc++;
						cTail[i].rect = rBoard[cTail[i].loc / 40][cTail[i].loc % 40];
						if (cTail[i].loc % 40 == 39)
							while (cTail[i].main == 1)
								cTail[i].main = rand() % 4;
						break;
					case 2:
						cTail[i].loc -= 40;
						cTail[i].rect = rBoard[cTail[i].loc / 40][cTail[i].loc % 40];
						if (cTail[i].loc / 40 == 0)
							while (cTail[i].main == 2)
								cTail[i].main = rand() % 4;
						break;
					case 3:
						cTail[i].loc += 40;
						cTail[i].rect = rBoard[cTail[i].loc / 40][cTail[i].loc % 40];
						if (cTail[i].loc / 40 == 39)
							while (cTail[i].main == 3)
								cTail[i].main = rand() % 4;
						break;
					}
					if (cTail[i].loc - 1 == cMain.loc
						|| cTail[i].loc + 1 == cMain.loc
						|| cTail[i].loc - 40 == cMain.loc
						|| cTail[i].loc + 40 == cMain.loc)
					{
						cTail[i].tail = cMain.tail;
						cMain.tail++;
					}
				}
				else if (cTail[i].tail == 0)
					cTail[i].rect = cMain.rect;
				else
					for (int j = 0; j < iSc; ++j)
						if (cTail[i].tail - 1 == cTail[j].tail)
							cTail[i].rect = cTail[j].rect;
				if (cTail[i].loc < 0 || cTail[i].loc > 1599)
					cTail[i].main = 5;
			}
			switch (cMain.main)
			{
			case 0:		// 좌로 이동
				if (cMain.loc == 0)
				{
					cMain.main = 1;
					cMain.sub = 3;
					cMain.loc++;
					cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					break;
				}
				else if (cMain.loc % 40 == 0)
				{
					if (cMain.sub == 2)
					{
						cMain.loc -= 40;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					else if (cMain.sub == 3)
					{
						cMain.loc += 40;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					cMain.main = 1;
					break;
				}
				cMain.loc--;
				cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
				break;
			case 1:		// 우로 이동
				if (cMain.loc == 1599)
				{
					cMain.main = 0;
					cMain.sub = 2;
					cMain.loc--;
					cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					break;
				}
				else if (cMain.loc % 40 == 39)
				{
					if (cMain.sub == 2)
					{
						cMain.loc -= 40;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					else if (cMain.sub == 3)
					{
						cMain.loc += 40;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					cMain.main = 0;
					break;
				}
				cMain.loc++;
				cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
				break;
			case 2:		// 상으로 이동
				if (cMain.loc == 0)
				{
					cMain.main = 3;
					cMain.sub = 1;
					cMain.loc += 40;
					cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					break;
				}
				else if (cMain.loc / 40 == 0)
				{
					if (cMain.sub == 0)
					{
						cMain.loc--;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					else if (cMain.sub == 1)
					{
						cMain.loc++;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					cMain.main = 3;
					break;
				}
				cMain.loc -= 40;
				cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
				break;
			case 3:		// 하로 이동
				if (cMain.loc == 1599)
				{
					cMain.main = 2;
					cMain.sub = 0;
					cMain.loc -= 40;
					cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					break;
				}
				if (cMain.loc / 40 == 39)
				{
					if (cMain.sub == 0)
					{
						cMain.loc--;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					else if (cMain.sub == 1)
					{
						cMain.loc++;
						cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
					}
					cMain.main = 2;
					break;
				}
				cMain.loc += 40;
				cMain.rect = rBoard[cMain.loc / 40][cMain.loc % 40];
				break;
			}
			if (iSpeed >= 0)
				iSpeed--;
			break;
		}
		InvalidateRect(hwnd, &rClient, true);
		break;
	case WM_LBUTTONDOWN:
		if (InCircle(LOWORD(lParam), HIWORD(lParam), cMain.rect))
		{
			if (iSpeed < 0 && cMain.sp != 10)
			{
				iSpeed = 10;
				cMain.sp -= 10;
				SetTimer(hwnd, 2, cMain.sp, NULL);
			}
			break;
		}
		for (int i = 0; i < iSc; ++i)
			if (InCircle(LOWORD(lParam), HIWORD(lParam), cTail[i].rect))
			{
				cTail[i].tail = -1;
				cTail[i].main = rand() % 4;
				break;
			}
		if (LOWORD(lParam) < cMain.rect.left)
		{
			cMain.main = 0;
			if (HIWORD(lParam) < cMain.rect.top)
				cMain.sub = 2;
			else
				cMain.sub = 3;
		}
		else if (LOWORD(lParam) > cMain.rect.right)
		{
			cMain.main = 1;
			if (HIWORD(lParam) < cMain.rect.top)
				cMain.sub = 2;
			else
				cMain.sub = 3;
		}
		else if (HIWORD(lParam) < cMain.rect.top)
			cMain.main = 2;
		else if (HIWORD(lParam) > cMain.rect.bottom)
			cMain.main = 3;

		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'Q':
		case 'q':
			PostQuitMessage(0);
			break;
		case 'S':
		case 's':
			if (bStart == false)
				SetTimer(hwnd, 2, cMain.sp, NULL);
			bStart = true;
			break;
		case VK_LEFT:
			cMain.main = 0;
			if (cMain.sub == 0 || cMain.sub == 1)
				2;
			break;
		case VK_RIGHT:
			cMain.main = 1;
			if (cMain.sub == 0 || cMain.sub == 1)
				3;
			break;
		case VK_UP:
			cMain.main = 2;
			if (cMain.sub == 2 || cMain.sub == 3)
				0;
			break;
		case VK_DOWN:
			cMain.main = 3;
			if (cMain.sub == 2 || cMain.sub == 3)
				1;
			break;
		case '+':
			if (cMain.sp > 10)
				cMain.sp -= 10;
			SetTimer(hwnd, 2, cMain.sp, NULL);
			break;
		case '-':
			if (cMain.sp < 200)
				cMain.sp += 10;
			SetTimer(hwnd, 2, cMain.sp, NULL);
			break;
		case 'J':
		case 'j':
			if (cMain.loc / 40 > 0)
			{
				cMain.loc -= 40;
				for (int i = 0; i < iSc; ++i)
					cTail[i].loc -= 40;
				InvalidateRect(hwnd, &rClient, true);
				cMain.loc += 40;
				for (int i = 0; i < iSc; ++i)
					cTail[i].loc += 40;
			}
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
