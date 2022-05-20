#pragma once
#include <iterator>
#include <Windows.h>

class MyImage
{
public:
	MyImage();
	
	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;
	RECT getRect() const;
	void setX(const int x);
	void setY(const int y);
	void setW(const int w);
	void setH(const int h);
	void reset();
private:
	int x, y;
	int width, height;
	RECT rect;

};

