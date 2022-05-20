#include "MyImage.h"

MyImage::MyImage() : x{}, y{}, width{}, height{}, rect{}
{
	
}

int MyImage::getX() const { return x; }

int MyImage::getY() const { return y; }

int MyImage::getW() const { return width; }

int MyImage::getH() const { return height; }

RECT MyImage::getRect() const { return rect; }

void MyImage::setW(const int w) { width = w; }

void MyImage::setH(const int h) { height = h; }

void MyImage::reset()
{
	x = {};
	y = {};
	width = {};
	height = {};
	rect = {};
}
