#ifndef SNOWBALLH
#define SNOWBALLH

#include "sprite.h"

class Snowball : public sprite::Sprite {
	int speed;
	float angle;

public:
	Snowball();
	void setAngle(double angle);
	void spawn(int x, int y) override;
	void update(Rectangle& viewport);
};

#endif