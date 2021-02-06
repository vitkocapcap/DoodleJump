#include "White_Platform.h"


White_Platform::White_Platform(ALLEGRO_BITMAP *img)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = NULL;
	x = y = dx = flag = 0;
	avail = true;
}


White_Platform::~White_Platform()
{
}

void White_Platform::affect(Player *player, std::list<Platform *> &platforms, int max_width) {
	std::list<Platform *>::iterator trace = platforms.begin();
	while (trace != platforms.end()) {
		if ((*trace)->code == code) (*trace)->set_x(rand() % (max_width - (*trace)->get_width()));
		trace++;
	}//all white platform will teleport
}
