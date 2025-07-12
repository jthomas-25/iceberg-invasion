#ifndef PARATROOPERH
#define PARATROOPERH

#include "sprite.h"

// returns a random number between start and end (inclusive)
int randrange(int start, int end);

class Paratrooper : public sprite::Sprite {
	enum class State {
		Parachute,
		Land,
		Fall
	};
	State state;
	int speed;
	int respawnTimer; // number of frames to wait before reappearing onscreen
	Sprite parachute;
	double angle; // degrees
	double dTheta; // angular velocity

	void handleState();

public:
	Paratrooper();
	void spawn(int x, int y) override;
	bool isParachuting();
	void land();
	void fall();
	void update(Rectangle& viewport);
	void draw() override;
};

#endif