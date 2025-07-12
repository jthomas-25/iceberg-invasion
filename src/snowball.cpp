#include "snowball.h"


Snowball::Snowball() : sprite::Sprite("snowball.png")
{
	speed = 0;
	angle = 0.0;
}

void Snowball::setAngle(double angle) {
	this->angle = angle;
}

void Snowball::spawn(int x, int y) {
	speed = 10;
	Sprite::spawn(x, y);
}

void Snowball::update(Rectangle& viewport) {
	if (alive) {
		// change the trajectory based on the angle
		x += speed * cos(angle);
		y -= speed * sin(angle);
		centerHitbox();
		// if the snowball goes offscreen, make it inactive
		if (x < 0 || x > viewport.getWidth() || y < 0) {
			alive = false;
		}
	}
}