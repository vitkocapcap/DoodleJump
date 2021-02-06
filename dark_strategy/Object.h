#pragma once
#include "Sprite.h"
#include "allegro5\allegro.h"
#include "allegro5\allegro_audio.h"

class Player;
class Platform;

class Object
{
protected:
	Sprite *current, *standard, *subordinate;
	Timer *timer;
	ALLEGRO_SAMPLE *sound;
	float x, y;
public:
	bool trans = false;
	friend Platform;
	friend Player;
	Object();
	~Object();
	int get_width();
	int get_height();
	void set_audio(ALLEGRO_SAMPLE *sound);
	virtual void effect(Player *player) = 0;
	virtual bool allow(Player *player) = 0;
	virtual bool collide(Player *player, int max_width) = 0;
	virtual void show(int flag, float tint_percent) = 0;
	virtual bool time_out() = 0;
	//virtual function for all the object can use
};

