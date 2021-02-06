#pragma once
#include "Object.h"
#include "Player.h"

class Spring :
	public Object
{
public:
	Spring(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2);
	void show(int flag, float tint_percent);
	void effect(Player *player);
	bool allow(Player *player);
	bool collide(Player *player, int max_width);
	bool time_out();
	~Spring();
};

