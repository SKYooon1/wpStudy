#pragma once

#include <tchar.h>
#include "myFunction.h"
#include "resource.h"

static HINSTANCE globalHInstance;
static LPCTSTR lpszClass = L"Window Class Name";
static LPCTSTR lpszWindowName = L"Windows program 5_1";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

LRESULT CALLBACK wndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
