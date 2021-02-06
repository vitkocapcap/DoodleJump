#include "Yellow_Platform.h"

Yellow_Platform::Yellow_Platform(ALLEGRO_BITMAP *img)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = NULL;
	x = y = dx = flag = 0;
	avail = true;
	attach = NULL;
}


Yellow_Platform::~Yellow_Platform()
{
}

void Yellow_Platform::affect(Player *player, std::list<Platform *> &platforms, int max_width) {
	end = true;
}//return true for disappear platform
