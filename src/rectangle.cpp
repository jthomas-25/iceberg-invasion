#include "rectangle.h"


Rectangle::Rectangle(int x, int y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	cx = x + width / 2;
	cy = y + height / 2;
}

int Rectangle::getX() { return x; }

int Rectangle::getY() { return y; }

int Rectangle::getWidth() { return width; }

int Rectangle::getHeight() { return height; }

int Rectangle::getCenterX() { return cx; }

int Rectangle::getCenterY() { return cy; }

void Rectangle::setX(int value) {
	// move rectangle horizontally
	x = value;
	cx = x + width / 2;
}

void Rectangle::setY(int value) {
	// move rectangle vertically
	y = value;
	cy = y + height / 2;
}

void Rectangle::setWidth(int value) {
	// resize around center
	if (value > 0) {
		width = value;
		x = cx - width / 2;
	}
}

void Rectangle::setHeight(int value) {
	// resize around center
	if (value > 0) {
		height = value;
		y = cy - height / 2;
	}
}

void Rectangle::setCenterX(int value) {
	// move rectangle horizontally
	cx = value;
	x = cx - width / 2;
}

void Rectangle::setCenterY(int value) {
	// move rectangle vertically
	cy = value;
	y = cy - height / 2;
}

void Rectangle::scale(double xMul, double yMul) {
	setWidth(width * xMul);
	setHeight(height * yMul);
}

bool Rectangle::intersects(Rectangle& other) {
	return x < other.x + other.width &&
		x + width > other.x &&
		y < other.y + other.height &&
		y + height > other.y;
}