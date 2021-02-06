#include "Green_Platform.h"



Green_Platform::Green_Platform(ALLEGRO_BITMAP *img)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = NULL;
	x = y = dx = flag = 0;
	avail = true;
	attach = NULL;
}


Green_Platform::~Green_Platform()
{
}

void Green_Platform::affect(Player *player, std::list<Platform *> &platforms, int max_width) {
	//do_nothing
}
