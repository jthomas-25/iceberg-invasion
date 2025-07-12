#include "sprite.h"


double radians(double degrees) {
	return degrees * (PI / 180);
}

// maps sprite image files to their original bitmaps for fast retrieval and copying
std::map<std::string, ALLEGRO_BITMAP*> sprite::Sprite::graphics;

sprite::Sprite::Sprite(const std::string& imageFilename) :
	x(0),
	y(0),
	alive(false),
	image(loadImage(imageFilename)),
	hitbox(0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image))
{
}

sprite::Sprite::~Sprite() {
	al_destroy_bitmap(image);
}

ALLEGRO_BITMAP* sprite::Sprite::loadImage(const std::string& filename) {
	// copy sprite image if it already exists in the map; otherwise, load it from disk
	if (graphics.count(filename) == 1) {
		return al_clone_bitmap(graphics.at(filename));
	}
	else {
		// this means the Sprite class can still be used in other programs
		return al_load_bitmap(filename.c_str());
	}
}

void sprite::Sprite::setAlpha(ALLEGRO_COLOR color) {
	al_convert_mask_to_alpha(image, color);
}

int sprite::Sprite::getX() { return x; }

int sprite::Sprite::getY() { return y; }

bool sprite::Sprite::isAlive() { return alive; }

Rectangle& sprite::Sprite::getHitbox() { return this->hitbox; }

void sprite::Sprite::centerHitbox() {
	hitbox.setCenterX(x);
	hitbox.setCenterY(y);
}

void sprite::Sprite::spawn(int x, int y) {
	alive = true;
	this->x = x;
	this->y = y;
}

void sprite::Sprite::die() { alive = false; }

void sprite::Sprite::moveX(int dx) {
	x += dx;
}

void sprite::Sprite::moveY(int dy) {
	y += dy;
}

void sprite::Sprite::draw() {
	if (alive) {
		int w = al_get_bitmap_width(image);
		int h = al_get_bitmap_height(image);
		al_draw_bitmap(image, x - w / 2, y - h / 2, 0);
		//drawHitbox(colors::RED);
	}
}

void sprite::Sprite::drawRotated(float cx, float cy, float dx, float dy, double angle, int flags) {
	if (alive) {
		al_draw_rotated_bitmap(image, cx, cy, dx, dy, angle, flags);
		//drawHitbox(colors::RED);
	}
}

void sprite::Sprite::drawHitbox(ALLEGRO_COLOR color) {
	int x = hitbox.getX();
	int y = hitbox.getY();
	al_draw_rectangle(x, y, x + hitbox.getWidth(), y + hitbox.getHeight(), color, 1);
}

bool sprite::collide_sprite(sprite::Sprite& sprite1, sprite::Sprite& sprite2) {
	// check for collision only if both sprites are active
	if (sprite1.isAlive() && sprite2.isAlive()) {
		return sprite1.getHitbox().intersects(sprite2.getHitbox());
	}
	else {
		return false;
	}
}