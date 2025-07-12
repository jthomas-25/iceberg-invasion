#include "player.h"


Player::Player() : Sprite("player.png"),
	cannon()
{
}

void Player::spawn(int x, int y) {
	Sprite::spawn(x, y);
	centerHitbox();
	cannon.spawn(x, y - 32);
	cannon.centerHitbox();
}

void Player::rotateCannonLeft() {
	cannon.rotateLeft();
}

void Player::rotateCannonRight() {
	cannon.rotateRight();
}

void Player::fireSnowball(Snowball& snowball) {
	cannon.fire(snowball);
}

void Player::update() {
	cannon.update();
}

void Player::draw() {
	cannon.draw();
	al_draw_filled_ellipse(x, y + 40, 44, 12, colors::GRAY);
	Sprite::draw();
}


Cannon::Cannon() : Sprite("cannon.png")
{
	rotationSpeed = 1.5;
	angle = 90.0;
	cooldown = 0;
}

void Cannon::rotateLeft() {
	angle += rotationSpeed;
	// stop rotating counterclockwise
	if (angle > 150) {
		angle = 150;
	}
}

void Cannon::rotateRight() {
	angle -= rotationSpeed;
	// stop rotating clockwise
	if (angle < 30) {
		angle = 30;
	}
}

void Cannon::fire(Snowball& snowball) {
	if (cooldown == 0 && !snowball.isAlive()) {
		double cannonAngle = radians(angle);
		snowball.setAngle(cannonAngle);
		// position snowball at the end of the cannon
		double r = hypot(al_get_bitmap_width(image) / 2, al_get_bitmap_height(image) / 2);
		int x = this->x + r * cos(cannonAngle);
		int y = this->y - r * sin(cannonAngle);
		snowball.spawn(x, y);
		cooldown = 15;
	}
}

void Cannon::update() {
	if (cooldown > 0) {
		cooldown--;
	}
}

void Cannon::draw() {
	int w = al_get_bitmap_width(image);
	int h = al_get_bitmap_height(image);
	// rotate cannon image around its center (relative to itself)
	double dTheta = PI / 2 - radians(angle);
	drawRotated(w / 2, h / 2, x, y, dTheta, 0);
}