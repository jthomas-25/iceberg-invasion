#ifndef SPRITEH
#define SPRITEH

#include <map>
#include <cmath>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "rectangle.h"

namespace colors {
    const ALLEGRO_COLOR BLACK = al_map_rgb(0, 0, 0);
    const ALLEGRO_COLOR GRAY = al_map_rgb(128, 128, 128);
    const ALLEGRO_COLOR MAGENTA = al_map_rgb(255, 0, 255);
    const ALLEGRO_COLOR RED = al_map_rgb(255, 0, 0);
    const ALLEGRO_COLOR WHITE = al_map_rgb(255, 255, 255);
}

const double PI = 3.14;
double radians(double degrees);

namespace sprite {
    class Sprite {
    protected:
        int x; // center x coordinate
        int y; // center y coordinate
        bool alive; // controls whether to update, draw, and handle collisions with the sprite
        ALLEGRO_BITMAP* image;
        Rectangle hitbox;

        static ALLEGRO_BITMAP* loadImage(const std::string& filename);
        void drawHitbox(ALLEGRO_COLOR color); // debug method

    public:
        static std::map<std::string, ALLEGRO_BITMAP*> graphics;
        Sprite() = default;
        Sprite(const std::string& imageFilename);
        virtual ~Sprite();

        void setAlpha(ALLEGRO_COLOR color); // sets the transparent color
        int getX();
        int getY();
        bool isAlive();
        Rectangle& getHitbox();
        void centerHitbox();
        virtual void spawn(int x, int y);
        void die();
        void moveX(int dx);
        void moveY(int dy);
        virtual void draw();
        void drawRotated(float cx, float cy, float dx, float dy, double angle, int flags);
    };

    // checks for sprite-sprite collision
    bool collide_sprite(Sprite& sprite1, Sprite& sprite2);
}

#endif
