#pragma once
#include "Object.h"
#include "Player.h"

class Rocket :
	public Object
{
public:
	friend Player;
	Rocket(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2);
	void show(int flag, float tint_percent);
	void effect(Player *player);
	bool allow(Player *player);
	bool collide(Player *player, int max_width);
	bool time_out();
	~Rocket();
};

