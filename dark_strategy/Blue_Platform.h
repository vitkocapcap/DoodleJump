#pragma once
#include "Platform.h"
class Blue_Platform :
	public Platform
{
public:
	Blue_Platform(ALLEGRO_BITMAP *img);
	~Blue_Platform();
	void affect(Player *player, std::list<Platform *> &platforms, int max_width) override;
	void show(float tint_percent);
};

