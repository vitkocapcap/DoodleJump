#pragma once
#include <allegro5\allegro.h>
#include "Timer.h"

class Sprite
{
private:
	ALLEGRO_BITMAP *image;
	Timer *timer;
	int current, quantity, width, height;
public:
	bool allow = true;
	Sprite(const char *filename, int quantity, double delay);
	Sprite(ALLEGRO_BITMAP *img, int quantity, double delay);
	~Sprite();

	void draw(float x, float y, int flag = 0, float tint_percent = 1);
	void next();

	int get_width();
	int get_height();
	int get_quantity();
	void get_current_of(Sprite *);
};

