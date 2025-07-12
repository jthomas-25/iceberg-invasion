#include "paratrooper.h"


int randrange(int start, int end) {
    return start + rand() % (end - start + 1);
}

Paratrooper::Paratrooper() : sprite::Sprite("paratrooper.png"),
    parachute("parachute.png")
{
    hitbox.setWidth(64);
    state = State::Parachute;
    speed = 0;
    respawnTimer = randrange(30, 60);
    angle = 0.0;
    dTheta = 0.0;
}

void Paratrooper::spawn(int x, int y) {
    if (respawnTimer == 0) {
        Sprite::spawn(x, y);
        parachute.spawn(x, y - 64);
        state = State::Parachute;
        speed = randrange(1, 3);
        respawnTimer = randrange(30, 60);
        angle = 5;
    }
}

bool Paratrooper::isParachuting() {
    return state == State::Parachute;
}

void Paratrooper::land() {
    parachute.die();
    state = State::Land;
}

void Paratrooper::fall() {
    parachute.die();
    state = State::Fall;
}

void Paratrooper::handleState() {
    switch (state) {
    case State::Parachute:
        parachute.moveY(speed);
        parachute.centerHitbox();
        // sway slowly
        dTheta += -0.05 * sin(radians(angle));
        angle += dTheta;
        break;
    case State::Land:
        speed = 0;
        angle = 0.0;
        break;
    case State::Fall:
        speed++;
        if (speed > 16) {
            speed = 16;
        }
        angle = 0.0;
        break;
    }
}

void Paratrooper::update(Rectangle& viewport) {
    if (alive) {
        handleState();
        moveY(speed);
        centerHitbox();
        // if the penguin goes offscreen, make it inactive
        if (y > viewport.getHeight()) {
            alive = false;
        }
    }
    else {
        respawnTimer--;
    }
}

void Paratrooper::draw() {
    if (alive) {
        int w = al_get_bitmap_width(image);
        int h = al_get_bitmap_height(image);
        double theta = radians(angle);
        parachute.drawRotated(w / 2, h / 2, parachute.getX(), parachute.getY(), theta / 2, 0);
        int flipFlag = (state == State::Fall) ? ALLEGRO_FLIP_VERTICAL : 0;
        drawRotated(w / 2, h / 2, x, y, theta, flipFlag);
    }
}