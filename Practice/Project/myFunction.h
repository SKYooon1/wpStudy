#pragma once

#include <vector>
#include "MyImage.h"

void resetImages(std::vector<MyImage>& images, const int wPrint, const int hPrint, const char split);

BOOL inBox(const int x, const int y, const RECT rect);

int selectBox(WORD param, RECT rect);
