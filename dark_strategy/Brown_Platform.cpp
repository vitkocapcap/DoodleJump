#include "Brown_Platform.h"
#include <iostream>


Brown_Platform::Brown_Platform(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = new Sprite(img2, 6, 0.01);
	//loading image
	subordinate->allow = false;
	x = y = dx = flag = 0;
	avail = false;
	attach = NULL;
}

Brown_Platform::~Brown_Platform()
{
}

void Brown_Platform::affect(Player *player, std::list<Platform *> &platforms, int max_width) {
	if (current == standard) x -= (subordinate->get_width() - standard->get_width()) / 2;
	current = subordinate;
	self_tint = 0.1;
}//disapper brown platform
