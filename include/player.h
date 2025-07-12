#ifndef PLAYERH
#define PLAYERH

#include "sprite.h"
#include "snowball.h"

class Cannon : public sprite::Sprite {
	double angle; // degrees
	double rotationSpeed;
	int cooldown; // number of frames until next shot can be fired

public:
	Cannon();
	void rotateLeft();
	void rotateRight();
	void fire(Snowball& snowball);
	void update();
	void draw() override;
};

class Player : public sprite::Sprite {
	Cannon cannon;

public:
	Player();
	void spawn(int x, int y) override;
	void rotateCannonLeft();
	void rotateCannonRight();
	void fireSnowball(Snowball& snowball);
	void update();
	void draw() override;
};

#endif