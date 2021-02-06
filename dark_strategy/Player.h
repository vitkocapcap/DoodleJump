#pragma once
#include "Character.h"
#include "Sprite.h"
#include "Object.h"

class Rocket;

class Player :
	public Character
{
private:
	Sprite *current, *jumping, *falling, *shooting;
	Object *attach;
	//create object and sprite, actually have no shooting
	int flag = 0;
public:
	friend Platform;
	friend Object;
	friend Rocket;

	Player(ALLEGRO_CONFIG *cfg, double delay);
	~Player();

	void show(float tint_percent, float max_width);
	void update(float gravity);

	int get_width();
	int get_height();
	float get_y();
	float get_x();
	int get_flag();

	bool is_falling();
	bool die(int max_height);

	void set_y(float y);
	void set_location(float x, float y);
	void set_dy(float dy);
	void set_dx(float dx);
	void set_jumping();
	void set_flag(int flag);

	float get_dx();
	float get_dy();
};

