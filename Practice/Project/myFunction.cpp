#include "myFunction.h"

void resetImages(std::vector<MyImage>& images, const int wPrint, const int hPrint, const char split)
{
	switch (split)
	{
	case '1':
		for (MyImage image : images)
			image.reset();

		images[0].setWh(wPrint, hPrint);
		break;
	case '2':
		for (MyImage image : images)
			image.reset();

		images[1].setX(0);
		images[1].setY(hPrint / 2);
		images[2].setX(wPrint / 2);
		images[2].setY(0);
		images[3].setX(wPrint / 2);
		images[3].setY(hPrint / 2);

		for (int i = 0; i < 4; ++i)
			images[i].setWh(wPrint / 2, hPrint / 2);
		break;
	case '3':
		for (MyImage image : images)
		{
			image.reset();
			image.setWh(wPrint / 3, hPrint / 3);
		}

		for (int i = 0; i < 9; i = i + 3)
		{
			images[i + 1].setX(wPrint / 3);
			images[i + 2].setX(wPrint * 2 / 3);
		}

		for (int i = 0; i < 3; ++i)
		{
			images[i].setY()
		}
		
		break;
	}
	return;
}

BOOL inBox(const int x, const int y, const RECT rect)
{
	if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom)
		return true;
	else return false;
}

int selectBox(WORD param, RECT rect)
{
	return {};
}
