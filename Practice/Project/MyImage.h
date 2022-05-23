#pragma once
#include <iterator>
#include <Windows.h>

class MyImage
{
public:
	MyImage();

	bool isSelected() const;

	int getX() const;
	int getY() const;
	int getW() const;
	int getH() const;

	RECT getRect();

	void setX(const int x);
	void setY(const int y);
	void setWh(const int w, const int h);
	void setSelected(const bool b);

	void reset();
private:
	int x_;
	int y_;
	int width_;
	int height_;
	RECT rect_;
	bool isSelected_;
};

