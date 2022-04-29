#include <windows.h>
#include <tchar.h>
#include <random>

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Windows program 2_11";

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

struct Shape		// 도형
{
	char sh{};		// 모양
	bool ch{};		// 선택
	bool bh{};		// 크기
	int rgb[3]{};	// 색
	int loc{};		// 좌표 인덱스
	RECT rect{};	// RECT
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;

	static RECT rClient{};
	static RECT rect[50][50]{};
	static Shape sShape[5];
	static int size{};

	POINT tri[3]{};

	srand((unsigned int)time(0));

	// 메시지 처리하기
	switch (uMsg)
	{										// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hwnd, &rClient);
		size = 40;

		for (int i = 0; i < size; ++i)
		{
			for (int j = 0; j < size; ++j)
			{
				rect[i][j] = {
					j * (rClient.right / size),
					i * (rClient.bottom / size),
					(j + 1) * (rClient.right / size),
					(i + 1) * (rClient.bottom / size)
				};
			}
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				Rectangle(hdc, rect[i][j].left, rect[i][j].top,
					rect[i][j].right, rect[i][j].bottom);

		for (int i = 0; i < 5; ++i)
		{
			switch (sShape[i].sh)
			{
			case 'E':		// 원
				hBrush = CreateSolidBrush(RGB(sShape[i].rgb[0],
					sShape[i].rgb[1], sShape[i].rgb[2]));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				if (sShape[i].ch == true)
				{
					hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Ellipse(hdc, sShape[i].rect.left,
						sShape[i].rect.top,
						sShape[i].rect.right,
						sShape[i].rect.bottom);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else
					Ellipse(hdc, sShape[i].rect.left,
						sShape[i].rect.top,
						sShape[i].rect.right,
						sShape[i].rect.bottom);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			case 'T':		// 삼각형
				tri[0] = { (sShape[i].rect.left / 2) +
					(sShape[i].rect.right / 2),
					sShape[i].rect.top };
				tri[1] = { sShape[i].rect.left,sShape[i].rect.bottom };
				tri[2] = { sShape[i].rect.right,sShape[i].rect.bottom };
				hBrush = CreateSolidBrush(RGB(sShape[i].rgb[0],
					sShape[i].rgb[1], sShape[i].rgb[2]));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				if (sShape[i].ch == true)
				{
					hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Polygon(hdc, tri, 3);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else
					Polygon(hdc, tri, 3);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			case 'R':
				hBrush = CreateSolidBrush(RGB(sShape[i].rgb[0],
					sShape[i].rgb[1], sShape[i].rgb[2]));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				if (sShape[i].ch == true)
				{
					hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, sShape[i].rect.left, sShape[i].rect.top,
						sShape[i].rect.right, sShape[i].rect.bottom);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else
					Rectangle(hdc, sShape[i].rect.left, sShape[i].rect.top,
						sShape[i].rect.right, sShape[i].rect.bottom);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				break;
			}
		}

		EndPaint(hwnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'S':
		case 's':
			size = 30;
			for (int i = 0; i < 5; ++i)
			{
				sShape[i].sh = 0;
				sShape[i].ch = 0;
				sShape[i].bh = 0;
				sShape[i].loc = 0;
				sShape[i].rect = {};
			}
			for (int i = 0; i < size; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					rect[i][j] = {
						j * (rClient.right / size),
						i * (rClient.bottom / size),
						(j + 1) * (rClient.right / size),
						(i + 1) * (rClient.bottom / size)
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'M':
		case 'm':
			size = 40;
			for (int i = 0; i < 5; ++i)
			{
				sShape[i].sh = 0;
				sShape[i].ch = 0;
				sShape[i].bh = 0;
				sShape[i].loc = 0;
				sShape[i].rect = {};
			}
			for (int i = 0; i < size; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					rect[i][j] = {
						j * (rClient.right / size),
						i * (rClient.bottom / size),
						(j + 1) * (rClient.right / size),
						(i + 1) * (rClient.bottom / size)
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'L':
		case 'l':
			size = 50;
			for (int i = 0; i < 5; ++i)
			{
				sShape[i].sh = 0;
				sShape[i].ch = 0;
				sShape[i].bh = 0;
				sShape[i].loc = 0;
				sShape[i].rect = {};
			}
			for (int i = 0; i < size; ++i)
			{
				for (int j = 0; j < size; ++j)
				{
					rect[i][j] = {
						j * (rClient.right / size),
						i * (rClient.bottom / size),
						(j + 1) * (rClient.right / size),
						(i + 1) * (rClient.bottom / size)
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'E':
		case 'e':
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].sh == 0)
				{
					sShape[i].sh = 'E';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
					break;
				}
				else if (i == 4)
				{
					for (int j = 0; j < 4; ++j)
						sShape[j] = sShape[j + 1];
					sShape[i].sh = 'E';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'T':
		case 't':
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].sh == 0)
				{
					sShape[i].sh = 'T';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
					break;
				}
				else if (i == 4)
				{
					for (int j = 0; j < 4; ++j)
						sShape[j] = sShape[j + 1];
					sShape[i].sh = 'T';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'R':
		case 'r':
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].sh == 0)
				{
					sShape[i].sh = 'R';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
					break;
				}
				else if (i == 4)
				{
					for (int j = 0; j < 4; ++j)
						sShape[j] = sShape[j + 1];
					sShape[i].sh = 'R';
					sShape[i].rgb[0] = rand() % 256;
					sShape[i].rgb[1] = rand() % 256;
					sShape[i].rgb[2] = rand() % 256;
					sShape[i].loc = rand() % (size * size);
					sShape[i].rect = {
						rect[sShape[i].loc / size][sShape[i].loc % size].left,
						rect[sShape[i].loc / size][sShape[i].loc % size].top,
						rect[sShape[i].loc / size][sShape[i].loc % size].right,
						rect[sShape[i].loc / size][sShape[i].loc % size].bottom
					};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case '1':
			for (int i = 0; i < 5; ++i)
				sShape[i].ch = 0;
			sShape[0].ch = true;
			InvalidateRect(hwnd, NULL, true);
			break;
		case '2':
			for (int i = 0; i < 5; ++i)
				sShape[i].ch = 0;
			sShape[1].ch = true;
			InvalidateRect(hwnd, NULL, true);
			break;
		case '3':
			for (int i = 0; i < 5; ++i)
				sShape[i].ch = 0;
			sShape[2].ch = true;
			InvalidateRect(hwnd, NULL, true);
			break;
		case '4':
			for (int i = 0; i < 5; ++i)
				sShape[i].ch = 0;
			sShape[3].ch = true;
			InvalidateRect(hwnd, NULL, true);
			break;
		case '5':
			for (int i = 0; i < 5; ++i)
				sShape[i].ch = 0;
			sShape[4].ch = true;
			InvalidateRect(hwnd, NULL, true);
			break;
		case VK_UP:
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].loc < size)
						sShape[i].loc = (size * size) - size + sShape[i].loc;
					else
						sShape[i].loc -= size;
					sShape[i].rect = rect[sShape[i].loc / size][sShape[i].loc % size];
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case VK_DOWN:
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].loc > (size * size) - size - 1)
						sShape[i].loc = sShape[i].loc + size - (size * size);
					else
						sShape[i].loc += size;
					sShape[i].rect = rect[sShape[i].loc / size][sShape[i].loc % size];
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case VK_LEFT:
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].loc % size == 0)
						sShape[i].loc += size - 1;
					else
						--sShape[i].loc;
					sShape[i].rect = rect[sShape[i].loc / size][sShape[i].loc % size];
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case VK_RIGHT:
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].loc % size == size - 1)
						sShape[i].loc -= size - 1;
					else
						++sShape[i].loc;
					sShape[i].rect = rect[sShape[i].loc / size][sShape[i].loc % size];
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'c':
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].bh == false)
					{
						sShape[i].bh = true;
						sShape[i].rect = {
							sShape[i].rect.left,
							sShape[i].rect.top,
							(sShape[i].rect.right * 2) - sShape[i].rect.left,
							(sShape[i].rect.bottom * 2) - sShape[i].rect.top
						};
					}
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'v':
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					if (sShape[i].bh == true)
					{
						sShape[i].bh = false;
						sShape[i].rect = {
							sShape[i].rect.left,
							sShape[i].rect.top,
							(sShape[i].rect.right / 2) + (sShape[i].rect.left / 2),
							(sShape[i].rect.bottom / 2) + (sShape[i].rect.top / 2)
						};
					}
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case VK_DELETE:
			for (int i = 0; i < 5; ++i)
			{
				if (sShape[i].ch == true)
				{
					sShape[i].sh = 0;
					sShape[i].ch = 0;
					sShape[i].bh = 0;
					sShape[i].loc = 0;
					sShape[i].rect = {};
				}
			}
			InvalidateRect(hwnd, NULL, true);
			break;
		case 'Q':
		case 'q':
			PostQuitMessage(0);
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
