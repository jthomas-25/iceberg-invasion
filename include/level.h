#ifndef LEVELH
#define LEVELH

#include <array>
#include <allegro5/allegro_font.h>
#include "sprite.h"
#include "player.h"
#include "snowball.h"
#include "paratrooper.h"

class Level {
	enum class State {
		Title,
		Normal,
		Paused,
		Win,
		GameOver
	};
	static const int NUM_PARATROOPERS = 10; // max number of onscreen enemies
	static const int NUM_SNOWBALLS = 3; // max number of onscreen snowballs
	ALLEGRO_BITMAP* background;
	ALLEGRO_BITMAP* titleBg;
	ALLEGRO_FONT* titleFont;
	ALLEGRO_FONT* statusFont;
	State state;
	int enemiesLanded;
	int enemiesDefeated;
	// objects
	Rectangle viewport;
	sprite::Sprite iceberg;
	Player player;
	sprite::Sprite chest;
	std::array<Snowball, NUM_SNOWBALLS> snowballs;
	std::array<Paratrooper, NUM_PARATROOPERS> paratroopers;
	
	void togglePause();
	void handleCollisions();
	void checkIfEnded();
	void drawHud();

public:
	Level() = default;
	Level(int screenWidth, int screenHeight);
	~Level();
	bool load();
	void getEvent(ALLEGRO_EVENT& ev);
	void update(bool keys[], bool& done);
	void draw();
};

#endif