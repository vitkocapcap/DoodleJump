#pragma once
#include "Platform.h"
class Brown_Platform :
	public Platform
{
public:
	Brown_Platform(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2);
	~Brown_Platform();
	void affect(Player *player, std::list<Platform *> &platforms, int max_width);
};

