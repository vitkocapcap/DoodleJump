#include "Blue_Platform.h"



Blue_Platform::Blue_Platform(ALLEGRO_BITMAP *img)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = NULL;
	x = y = flag = 0;
	dx = 10;
	avail = true;
	attach = NULL;
}


Blue_Platform::~Blue_Platform()
{
}

void Blue_Platform::affect(Player *player, std::list<Platform *> &platforms, int max_width) {
	//do nothing
}