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
		WINDOW_WIDTH, WINDOW_HEIGHT,
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
	HBRUSH hBrush, oldBrush;
	static HBITMAP hBitmap;
	static RECT rClient{};

	static DWORD dwRop{ SRCCOPY };
	constexpr int imageWidth{ 640 }, imageHeight{ 426 };
	static int wPrint{ imageWidth }, hPrint{ imageHeight };
	static std::vector<MyImage> images{};
	static char rectDivided{ '1' };
	RECT tempRect = {};

	// 메시지 처리하기
	switch (iMessage)
	{	// 메시지 번호
	case WM_CREATE:
		GetClientRect(hWnd, &rClient);
		hBitmap = (HBITMAP)LoadBitmap(globalHInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		images.resize(9);
		images[0].setWh(wPrint, hPrint);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hMemDc = CreateCompatibleDC(hdc);
		SelectObject(hMemDc, hBitmap);

		for (const MyImage& image : images)
		{
			StretchBlt(hdc, image.getX(), image.getY(), image.getW(), image.getH(),
				hMemDc, 0, 0, imageWidth, imageHeight, dwRop);
		}

		DeleteDC(hMemDc);

		printSelected(images, hdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'Q':
		case 'q':
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'R':
		case 'r':
			if (dwRop == SRCCOPY)
				dwRop = NOTSRCCOPY;
			else dwRop = SRCCOPY;
			InvalidateRect(hWnd, &rClient, true);
			break;
		case 'A':
		case 'a':
			if (wPrint == imageWidth && hPrint == imageHeight)
			{
				wPrint = WINDOW_WIDTH;
				hPrint = WINDOW_HEIGHT;
			}
			else
			{
				wPrint = imageWidth;
				hPrint = imageHeight;
			}
			resetImages(images, wPrint, hPrint, rectDivided);
			InvalidateRect(hWnd, &rClient, true);
			break;
		case '1':
			resetImages(images, wPrint, hPrint, '1');
			rectDivided = '1';
			InvalidateRect(hWnd, &rClient, true);
			break;
		case '2':
			resetImages(images, wPrint, hPrint, '2');
			rectDivided = '2';
			InvalidateRect(hWnd, &rClient, true);
			break;
		case '3':
			resetImages(images, wPrint, hPrint, '3');
			rectDivided = '3';
			InvalidateRect(hWnd, &rClient, true);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		for (MyImage& image : images)
			image.setSelected(false);

		for (MyImage& image : images)
		{
			if (inBox(LOWORD(lParam), HIWORD(lParam), image.getRect()))
			{
				image.setSelected(true);
				break;
			}
		}

		InvalidateRect(hWnd, &rClient, true);
		break;
	case WM_KEYDOWN:
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
