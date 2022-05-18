#pragma once

#include <Windows.h>
#include <vector>

void printNew(std::vector<int>& xDest, std::vector<int>& yDest,
	std::vector<int>& wDest, std::vector<int>& hDest,
	const int wPrint, const int hPrint, char c);

BOOL inBox(int x, int y, RECT rect);
