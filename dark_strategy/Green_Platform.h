#pragma once
#include "Platform.h"

class Green_Platform :
	public Platform
{
public:
	Green_Platform(ALLEGRO_BITMAP *img);
	~Green_Platform();
	void affect(Player *player, std::list<Platform *> &platforms, int max_width) override;
};

