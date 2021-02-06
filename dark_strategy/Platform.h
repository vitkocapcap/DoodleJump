#pragma once
#include "Sprite.h"
#include <list>
#include "Player.h"
#include "Spring.h"
#include "Rocket.h"

class Player;

class Platform
{
protected:
	Sprite *current, *standard, *subordinate;
	float x, y, dx;
	int flag;
	bool end = false;
	float self_tint = 0;
	Object *attach = NULL;
public:
	bool avail;
	friend Player;
	int code;
	~Platform();

	void show(float tint_percent);
	
	void update(float offset, float width_limit);

	virtual void affect(Player *player, std::list<Platform *> &platforms, int max_width) = 0;
	void attach_affect(Player *player);

	void set_flag(int flag);
	void set_location(float x, float y);
	void set_dx(float dx);
	void set_x(float x);
	void set_code(int code);
	void set_attach(Object *obj);

	int get_width();
	int get_height();
	
	bool out_of_sight(int max_height);
	bool to_be_destroyed();
	bool attach_allow(Player *player);
	bool collide(Player *player, int max_width);
	bool collide_attach(Player *player, int max_width);

	void fix_as(Player *player);
	float get_x();
};

