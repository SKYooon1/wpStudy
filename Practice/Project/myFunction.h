#pragma once

#include <vector>
#include "MyImage.h"

void resetImages(std::vector<MyImage> images, const int wPrint, const int hPrint, const char split);

void printNew(std::vector<int>& xDest, std::vector<int>& yDest,
	std::vector<int>& wDest, std::vector<int>& hDest,
	const int wPrint, const int hPrint, char c);

BOOL inBox(int x, int y, RECT rect);

int selectBox(WORD param, RECT rect);
