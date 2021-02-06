#pragma once
#include "Platform.h"

class Yellow_Platform :
	public Platform
{
public:
	Yellow_Platform(ALLEGRO_BITMAP *img);
	~Yellow_Platform();
	void affect(Player *player, std::list<Platform *> &platforms, int max_width);
};

