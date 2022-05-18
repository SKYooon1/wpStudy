#include "myFunction.h"

void printNew(std::vector<int>& xDest, std::vector<int>& yDest,
              std::vector<int>& wDest, std::vector<int>& hDest,
              const int wPrint, const int hPrint, char c)
{
	switch (c)
	{
	case '1':
		for (int i = 0; i < 3; ++i)
		{
			xDest[i] = 0;
			yDest[i] = 0;
			wDest[i] = 0;
			hDest[i] = 0;
		}

		wDest[0] = wPrint;
		hDest[0] = hPrint;
		break;
	case '2':
		for (int i = 0; i < 3; ++i)
		{
			xDest[i] = 0;
			yDest[i] = 0;
			wDest[i] = wPrint / 2;
			hDest[i] = hPrint / 2;
		}

		xDest[1] = wPrint / 2;
		yDest[1] = hPrint / 2;
		wDest[2] = 0;
		hDest[2] = 0;
		break;
	case '3':
		for (int i = 0; i < 3; ++i)
		{
			wDest[i] = wPrint / 3;
			hDest[i] = hPrint / 3;
		}

		xDest[0] = 0;
		yDest[0] = 0;
		xDest[1] = wPrint / 3;
		yDest[1] = hPrint / 3;
		xDest[2] = wPrint * 2 / 3;
		yDest[2] = hPrint * 2 / 3;
		break;
	}
}

BOOL inBox(int x, int y, RECT rect)
{
	if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom)
		return true;
	else return false;
}
