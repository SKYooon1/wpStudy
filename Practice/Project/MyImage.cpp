#include "MyImage.h"

MyImage::MyImage() : x_{}, y_{}, width_{}, height_{}, rect_{}, isSelected_{} {}

bool MyImage::isSelected() const { return isSelected_; }

RECT MyImage::getRect()
{
	rect_.left = x_;
	rect_.top = y_;
	rect_.right = width_;
	rect_.bottom = height_;
	return rect_;
}

void MyImage::reset()
{
	x_ = {};
	y_ = {};
	width_ = {};
	height_ = {};
	isSelected_ = {};
}

void MyImage::moveX(const int x)
{
	x_ += x;
}

void MyImage::scaleWh(const int w, const int h)
{
	width_ += w;
	height_ += h;
}


int MyImage::getX() const { return x_; }

int MyImage::getY() const { return y_; }

int MyImage::getW() const { return width_; }

int MyImage::getH() const { return height_; }

void MyImage::setX(const int x) { x_ = x; }

void MyImage::setY(const int y) { y_ = y; }

void MyImage::setWh(const int w, const int h)
{
	width_ = w;
	height_ = h;
}

void MyImage::setSelected(const bool b) { isSelected_ = b; }
