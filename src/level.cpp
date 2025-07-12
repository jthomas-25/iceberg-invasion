#include "level.h"


Level::Level(int screenWidth, int screenHeight) :
    iceberg("iceberg.png"),
    player(),
    chest("chest.png"),
    viewport(0, 0, screenWidth, screenHeight)
{
    background = nullptr;
    titleBg = nullptr;
    titleFont = nullptr;
    statusFont = nullptr;
    state = State::Title;
    enemiesLanded = 0;
    enemiesDefeated = 0;
}

Level::~Level() {
    al_destroy_bitmap(background);
    al_destroy_bitmap(titleBg);
    al_destroy_font(titleFont);
    al_destroy_font(statusFont);
}

bool Level::load() {
    background = al_load_bitmap("assets/backgrounds/stage.png");
    if (!background) {
        return false;
    }
    titleBg = al_load_bitmap("assets/backgrounds/title.png");
    if (!titleBg) {
        return false;
    }
    al_convert_mask_to_alpha(titleBg, colors::MAGENTA);

    const char* fontFilename = "assets/fonts/CHICKEN Pie.ttf";
    titleFont = al_load_font(fontFilename, 48, 0);
    if (!titleFont) {
        return false;
    }
    statusFont = al_load_font(fontFilename, 36, 0);
    if (!statusFont) {
        return false;
    }

    int cx = viewport.getCenterX();
    iceberg.spawn(cx, 528);
    iceberg.centerHitbox();
    iceberg.getHitbox().scale(0.8, 0.25);
    player.spawn(cx, 500);
    chest.spawn(830, 395);
    chest.centerHitbox();

    return true;
}

static bool key_down(ALLEGRO_EVENT& ev, int keycode) {
    return ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == keycode;
}

void Level::getEvent(ALLEGRO_EVENT& ev) {
    if (key_down(ev, ALLEGRO_KEY_ENTER)) {
        switch (state) {
        case State::Title:
            state = State::Normal;
            break;
        case State::Normal:
        case State::Paused:
            togglePause();
            break;
        }
    }
}

void Level::togglePause() {
    state = (state == State::Normal) ? State::Paused : State::Normal;
}

void Level::update(bool keys[], bool& done) {
    switch (state) {
    case State::Normal:
        if (keys[ALLEGRO_KEY_SPACE]) {
            for (auto& snowball : snowballs) {
                player.fireSnowball(snowball);
            }
        }
        if (keys[ALLEGRO_KEY_LEFT]) {
            player.rotateCannonLeft();
        }
        else if (keys[ALLEGRO_KEY_RIGHT]) {
            player.rotateCannonRight();
        }
        
        player.update();

        for (auto& paratrooper : paratroopers) {
            paratrooper.spawn(randrange(0, viewport.getWidth()), randrange(-128, -64));
        }
        for (auto& paratrooper : paratroopers) {
            paratrooper.update(viewport);
        }
        for (auto& snowball : snowballs) {
            snowball.update(viewport);
        }

        handleCollisions();
        checkIfEnded();
        break;
    case State::Win:
    case State::GameOver:
        // display the win/lose message for 5 seconds before quitting
        al_rest(5.0);
        done = true;
        break;
    }
}

void Level::handleCollisions() {
    for (auto& snowball : snowballs) {
        for (auto& paratrooper : paratroopers) {
            if (paratrooper.isParachuting() && sprite::collide_sprite(snowball, paratrooper)) {
                snowball.die();
                paratrooper.fall();
                enemiesDefeated++;
            }
        }
    }

    for (auto& paratrooper : paratroopers) {
        if (paratrooper.isParachuting() && sprite::collide_sprite(paratrooper, iceberg)) {
            paratrooper.land();
            enemiesLanded++;
        }
    }
}

void Level::checkIfEnded() {
    if (enemiesDefeated == 100) {
        for (auto& paratrooper : paratroopers) {
            paratrooper.die();
        }
        state = State::Win;
    }
    else if (enemiesLanded == 5) {
        state = State::GameOver;
    }
}

void Level::drawHud() {
    int cx = viewport.getCenterX();
    int cy = viewport.getCenterY();
    switch (state) {
    case State::Title:
        al_draw_bitmap(titleBg, cx - al_get_bitmap_width(titleBg) / 2, cy - 220, 0);
        al_draw_text(titleFont, colors::WHITE, cx, cy - 190, ALLEGRO_ALIGN_CENTER, "Iceberg Invasion");
        al_draw_text(statusFont, colors::BLACK, cx, cy - 50, ALLEGRO_ALIGN_CENTER, "Press Enter");
        break;
    case State::Paused:
        al_draw_text(statusFont, colors::BLACK, cx, cy - 50, ALLEGRO_ALIGN_CENTER, "Paused");
        break;
    case State::Win:
        al_draw_text(statusFont, colors::BLACK, cx, cy - 50, ALLEGRO_ALIGN_CENTER, "You Won!");
        break;
    case State::GameOver:
        al_draw_text(statusFont, colors::BLACK, cx, cy - 50, ALLEGRO_ALIGN_CENTER, "Game Over");
        break;
    }
}

void Level::draw() {
    al_draw_bitmap(background, 0, 0, 0);

    iceberg.draw();

    chest.draw();

    for (auto& paratrooper : paratroopers) {
        paratrooper.draw();
    }
    for (auto& snowball : snowballs) {
        snowball.draw();
    }

    player.draw();

    drawHud();
}