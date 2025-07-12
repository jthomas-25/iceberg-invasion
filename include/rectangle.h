#ifndef RECTANGLEH
#define RECTANGLEH

class Rectangle {
	int x; // left
	int y; // top
	int width;
	int height;
	int cx; // center x
	int cy; // center y

public:
	Rectangle() = default;
	Rectangle(int x, int y, int width, int height);
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	int getCenterX();
	int getCenterY();
	void setX(int value);
	void setY(int value);
	void setWidth(int value);
	void setHeight(int value);
	void setCenterX(int value);
	void setCenterY(int value);
	void scale(double xMul, double yMul);
	bool intersects(Rectangle& other);
};

#endif