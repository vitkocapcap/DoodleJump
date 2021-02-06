#pragma once
#include "Platform.h"
class White_Platform :
	public Platform
{
public:
	White_Platform(ALLEGRO_BITMAP *img);
	~White_Platform();
	void affect(Player *player, std::list<Platform *> &platforms, int max_width) override;
};

