#include "myFunction.h"

void resetImages(std::vector<MyImage>& images, const int wPrint, const int hPrint, const char split)
{
	switch (split)
	{
	case '1':
		for (MyImage& image : images)
			image.reset();

		images[0].setWh(wPrint, hPrint);
		break;
	case '2':
		for (MyImage& image : images)
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
		for (MyImage& image : images)
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
			images[i + 3].setY(hPrint / 3);
			images[i + 6].setY(hPrint * 2 / 3);
		}
	}
}

BOOL inBox(const int x, const int y, const RECT rect)
{
	if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom)
		return true;
	else return false;
}

void printSelected(const std::vector<MyImage> images, const HDC hdc)
{
	if (images[0].isSelected())
		Rectangle(hdc, images[0].getX(), images[0].getY(), images[0].getW(), images[0].getH());
	else if (images[1].isSelected())
		Rectangle(hdc, images[1].getX(), images[1].getY(), images[1].getW(), images[1].getH());
	else if (images[2].isSelected())
		Rectangle(hdc, images[2].getX(), images[2].getY(), images[2].getW(), images[2].getH());
	else if (images[3].isSelected())
		Rectangle(hdc, images[3].getX(), images[3].getY(), images[3].getW(), images[3].getH());
	else if (images[4].isSelected())
		Rectangle(hdc, images[4].getX(), images[4].getY(), images[4].getW(), images[4].getH());
	else if (images[5].isSelected())
		Rectangle(hdc, images[5].getX(), images[5].getY(), images[5].getW(), images[5].getH());
	else if (images[6].isSelected())
		Rectangle(hdc, images[6].getX(), images[6].getY(), images[6].getW(), images[6].getH());
	else if (images[7].isSelected())
		Rectangle(hdc, images[7].getX(), images[7].getY(), images[7].getW(), images[7].getH());
	else if (images[8].isSelected())
		Rectangle(hdc, images[8].getX(), images[8].getY(), images[8].getW(), images[8].getH());
}
