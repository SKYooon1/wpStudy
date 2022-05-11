#include "wndProc.h"

class Circle
{
public:
	RECT rect{};
	int tail{};
	int mainRoute{};
	int subRoute{};
	int location{};
	int speed{};
	int rgb[3]{255, 255, 0};
	bool shapeRect{};

	void move(const RECT* board, const int direction)
	{
		switch (direction)
		{
		case '0':
			if (location == 0)
			{
				mainRoute = 1;
				subRoute = 3;
				location++;
				rect = board[location];
				return;
			}
			else if (location % 40 == 0)
			{
				if (subRoute == 2)
				{
					location -= 40;
					rect = board[location];
				}
				else if (subRoute == 3)
				{
					location += 40;
					rect = board[location];
				}
				mainRoute = 1;
				return;
			}
			location--;
			rect = board[location];
			break;
		}
	}
};

float lengthPts(const int x1, const int y1, const int x2, const int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

BOOL inCircle(const int x, const int y, const RECT rect)
{
	if (lengthPts(x, y, (rect.left / 2) + (rect.right / 2),
		(rect.top / 2) + (rect.bottom / 2)) < ((rect.right - rect.left) / 2))
		return true;
	else
		return false;
}

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	static RECT rClient{};
	static RECT rBoard[40][40]{};
	static Circle cMain{};
	static Circle cTail[20]{};
	static int iSc{};
	bool bTi = true;
	static bool bStart = false;

	// 메시지 처리하기
	switch (iMessage)
	{
		static int iSpeed;
		HBRUSH oldBrush;
		HBRUSH hBrush;
		HDC hdc;
		// 메시지 번호
	case WM_CREATE:							// 메시지에 따라 처리
		GetClientRect(hWnd, &rClient);

		srand(static_cast<unsigned>(time(nullptr)));

		SetTimer(hWnd, 1, 5000, nullptr);

		for (int i = 0; i < 40; ++i)
			for (int j = 0; j < 40; ++j)
				rBoard[i][j] = { rClient.right * j / 40,rClient.bottom * i / 40,
					rClient.right * (j + 1) / 40,rClient.bottom * (i + 1) / 40 };

		cMain.rect = rBoard[0][0];
		cMain.mainRoute = 1;
		cMain.subRoute = 3;
		cMain.tail = 0;
		cMain.location = 0;
		cMain.speed = 100;

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (const RECT(&rects)[40] : rBoard)
			for (const RECT rect : rects)
				Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

		hBrush = CreateSolidBrush(RGB(cMain.rgb[0], cMain.rgb[1], cMain.rgb[2]));
		oldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBrush));
		if (!cMain.shapeRect)
			Ellipse(hdc, cMain.rect.left, cMain.rect.top, cMain.rect.right, cMain.rect.bottom);
		else
			Rectangle(hdc, cMain.rect.left, cMain.rect.top, cMain.rect.right, cMain.rect.bottom);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = static_cast<HBRUSH>(SelectObject(hdc, hBrush));
		for (int i = 0; i <= iSc; ++i)
		{
			Ellipse(hdc, cTail[i].rect.left, cTail[i].rect.top, cTail[i].rect.right, cTail[i].rect.bottom);
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			for (int i = 0; i < iSc; ++i)
				if (cTail[i].rect.left == rBoard[20][20].left)
					bTi = false;
			if (cMain.rect.left == rBoard[20][20].left)
				bTi = false;
			if (iSc < 20 && bTi == true)
			{
				cTail[iSc].rect = rBoard[20][20];
				cTail[iSc].mainRoute = rand() % 4;
				cTail[iSc].tail = -1;
				cTail[iSc].location = 420;
				iSc++;
			}
			break;
		case 2:
			if (iSpeed == 0)
			{
				cMain.speed += 10;
				SetTimer(hWnd, 2, cMain.speed, nullptr);
			}
			for (int i = 0; i < iSc; ++i)
			{
				if (cTail[i].tail == -1)
				{
					switch (cTail[i].mainRoute)
					{
					case 0:
						cTail[i].location--;
						cTail[i].rect = rBoard[cTail[i].location / 40][cTail[i].location % 40];
						if (cTail[i].location % 40 == 0)
							while (cTail[i].mainRoute == 0)
								cTail[i].mainRoute = rand() % 4;
						break;
					case 1:
						cTail[i].location++;
						cTail[i].rect = rBoard[cTail[i].location / 40][cTail[i].location % 40];
						if (cTail[i].location % 40 == 39)
							while (cTail[i].mainRoute == 1)
								cTail[i].mainRoute = rand() % 4;
						break;
					case 2:
						cTail[i].location -= 40;
						cTail[i].rect = rBoard[cTail[i].location / 40][cTail[i].location % 40];
						if (cTail[i].location / 40 == 0)
							while (cTail[i].mainRoute == 2)
								cTail[i].mainRoute = rand() % 4;
						break;
					case 3:
						cTail[i].location += 40;
						cTail[i].rect = rBoard[cTail[i].location / 40][cTail[i].location % 40];
						if (cTail[i].location / 40 == 39)
							while (cTail[i].mainRoute == 3)
								cTail[i].mainRoute = rand() % 4;
						break;
					default:
						break;
					}
					if (cTail[i].location - 1 == cMain.location || cTail[i].location + 1 == cMain.location ||
						cTail[i].location - 40 == cMain.location || cTail[i].location + 40 == cMain.location)
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
				if (cTail[i].location < 0 || cTail[i].location > 1599)
					cTail[i].mainRoute = 5;
			}
			switch (cMain.mainRoute)
			{
			case 0:		// 좌로 이동
				if (cMain.location == 0)
				{
					cMain.mainRoute = 1;
					cMain.subRoute = 3;
					cMain.location++;
					cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					break;
				}
				else if (cMain.location % 40 == 0)
				{
					if (cMain.subRoute == 2)
					{
						cMain.location -= 40;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					else if (cMain.subRoute == 3)
					{
						cMain.location += 40;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					cMain.mainRoute = 1;
					break;
				}
				cMain.location--;
				cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
				break;
			case 1:		// 우로 이동
				if (cMain.location == 1599)
				{
					cMain.mainRoute = 0;
					cMain.subRoute = 2;
					cMain.location--;
					cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					break;
				}
				else if (cMain.location % 40 == 39)
				{
					if (cMain.subRoute == 2)
					{
						cMain.location -= 40;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					else if (cMain.subRoute == 3)
					{
						cMain.location += 40;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					cMain.mainRoute = 0;
					break;
				}
				cMain.location++;
				cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
				break;
			case 2:		// 상으로 이동
				if (cMain.location == 0)
				{
					cMain.mainRoute = 3;
					cMain.subRoute = 1;
					cMain.location += 40;
					cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					break;
				}
				else if (cMain.location / 40 == 0)
				{
					if (cMain.subRoute == 0)
					{
						cMain.location--;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					else if (cMain.subRoute == 1)
					{
						cMain.location++;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					cMain.mainRoute = 3;
					break;
				}
				cMain.location -= 40;
				cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
				break;
			case 3:		// 하로 이동
				if (cMain.location == 1599)
				{
					cMain.mainRoute = 2;
					cMain.subRoute = 0;
					cMain.location -= 40;
					cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					break;
				}
				if (cMain.location / 40 == 39)
				{
					if (cMain.subRoute == 0)
					{
						cMain.location--;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					else if (cMain.subRoute == 1)
					{
						cMain.location++;
						cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
					}
					cMain.mainRoute = 2;
					break;
				}
				cMain.location += 40;
				cMain.rect = rBoard[cMain.location / 40][cMain.location % 40];
				break;
			}
			if (iSpeed >= 0)
				iSpeed--;
			break;
		}
		InvalidateRect(hWnd, &rClient, true);
		break;
	case WM_LBUTTONDOWN:
		if (inCircle(LOWORD(lParam), HIWORD(lParam), cMain.rect))
		{
			if (iSpeed < 0 && cMain.speed != 10)
			{
				iSpeed = 10;
				cMain.speed -= 10;
				SetTimer(hWnd, 2, cMain.speed, nullptr);
			}
			break;
		}
		for (int i = 0; i < iSc; ++i)
			if (inCircle(LOWORD(lParam), HIWORD(lParam), cTail[i].rect))
			{
				cTail[i].tail = -1;
				cTail[i].mainRoute = rand() % 4;
				break;
			}
		if (LOWORD(lParam) < cMain.rect.left)
		{
			cMain.mainRoute = 0;
			if (HIWORD(lParam) < cMain.rect.top)
				cMain.subRoute = 2;
			else
				cMain.subRoute = 3;
		}
		else if (LOWORD(lParam) > cMain.rect.right)
		{
			cMain.mainRoute = 1;
			if (HIWORD(lParam) < cMain.rect.top)
				cMain.subRoute = 2;
			else
				cMain.subRoute = 3;
		}
		else if (HIWORD(lParam) < cMain.rect.top)
			cMain.mainRoute = 2;
		else if (HIWORD(lParam) > cMain.rect.bottom)
			cMain.mainRoute = 3;

		break;
	case WM_RBUTTONDOWN:
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
				SetTimer(hWnd, 2, cMain.speed, nullptr);
			bStart = true;
			break;
		case VK_LEFT:
			cMain.mainRoute = 0;
			if (cMain.subRoute == 0 || cMain.subRoute == 1)
				cMain.subRoute = 2;
			break;
		case VK_RIGHT:
			cMain.mainRoute = 1;
			if (cMain.subRoute == 0 || cMain.subRoute == 1)
				cMain.subRoute = 3;
			break;
		case VK_UP:
			cMain.mainRoute = 2;
			if (cMain.subRoute == 2 || cMain.subRoute == 3)
				cMain.subRoute = 0;
			break;
		case VK_DOWN:
			cMain.mainRoute = 3;
			if (cMain.subRoute == 2 || cMain.subRoute == 3)
				cMain.subRoute = 1;
			break;
		case '+':
			if (cMain.speed > 10)
				cMain.speed -= 10;
			SetTimer(hWnd, 2, cMain.speed, nullptr);
			break;
		case '-':
			if (cMain.speed < 200)
				cMain.speed += 10;
			SetTimer(hWnd, 2, cMain.speed, nullptr);
			break;
		case 'J':
		case 'j':
			if (cMain.location / 40 > 0)
			{
				cMain.location -= 40;
				for (int i = 0; i < iSc; ++i)
					cTail[i].location -= 40;
				InvalidateRect(hWnd, &rClient, true);
				cMain.location += 40;
				for (int i = 0; i < iSc; ++i)
					cTail[i].location += 40;
			}
			break;
		case 'T':
		case 't':

			break;
		default:
			break;
		}
		break;
		case WM_COMMAND:
			{
				switch (LOWORD(wParam))
				{
				case ID_GAME_START:
					if (bStart == false)
						SetTimer(hWnd, 2, cMain.speed, nullptr);
					bStart = true;
					InvalidateRect(hWnd, &rClient, true);
					SetTimer(hWnd, 1, cMain.speed, nullptr);
					break;
				case ID_GAME_RESET:
					bStart = false;
					iSpeed = 30;
					cMain.rgb[0] = 255;
					cMain.rgb[1] = 255;
					cMain.rgb[2] = 0;
					cMain.shapeRect = false;
					bStart = false;
					
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_GAME_END:
					PostQuitMessage(0);
					break;
				case ID_SPEED_FAST:
					iSpeed = 50;
					SetTimer(hWnd, 2, cMain.speed, nullptr);
					break;
				case ID_SPEED_MEDIUM:
					iSpeed = 30;
					SetTimer(hWnd, 2, cMain.speed, nullptr);
					break;
				case ID_SPEED_SLOW:
					iSpeed = 10;
					SetTimer(hWnd, 2, cMain.speed, nullptr);
					break;
				case ID_COLOR_CYAN:
					cMain.rgb[0] = 0;
					cMain.rgb[1] = 255;
					cMain.rgb[2] = 255;
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_COLOR_MAGENTA:
					cMain.rgb[0] = 255;
					cMain.rgb[1] = 0;
					cMain.rgb[2] = 255;
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_COLOR_YELLOW:
					cMain.rgb[0] = 255;
					cMain.rgb[1] = 255;
					cMain.rgb[2] = 0;
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_SHAPE_CIRCLE:
					cMain.shapeRect = false;
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_SHAPE_RECTANGLE:
					cMain.shapeRect = true;
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_MOVE_ON:
					bStart = true;
					SetTimer(hWnd, 2, 1000, nullptr);
					InvalidateRect(hWnd, &rClient, true);
					break;
				case ID_MOVE_OFF:
					bStart = false;
					break;
				}
			}
		break;
	case WM_DESTROY:						// 메시지에 따라 처리
		PostQuitMessage(0);
		break;
	default:
		break;
	}										// 처리할 메시지만 case문에 나열
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
	// case에서 정의되지 않은 메시지는 커널잎 처리하도록 메시지 전달
}
