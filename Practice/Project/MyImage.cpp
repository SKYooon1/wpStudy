#include "MyImage.h"

MyImage::MyImage() : x_{}, y_{}, width_{}, height_{}, rect_{}
{
	
}

int MyImage::getX() const { return x_; }

int MyImage::getY() const { return y_; }

int MyImage::getW() const { return width_; }

int MyImage::getH() const { return height_; }

RECT MyImage::getRect() const { return rect_; }

void MyImage::setX(const int x) { x_ = x; }

void MyImage::setY(const int y) { y_ = y; }

void MyImage::setWh(const int w, const int h) { width_ = w; height_ = h; }

void MyImage::reset()
{
	x_ = {};
	y_ = {};
	width_ = {};
	height_ = {};
	rect_ = {};
}
