#include "Rocket.h"
#include <iostream>


Rocket::Rocket(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2)
{
	current = standard = new Sprite(img, 1, 1);
	subordinate = new Sprite(img2, 5, 0.2);
	trans = true;
	timer = NULL;
}
//loading image for rocket before and after

Rocket::~Rocket()
{
}

void Rocket::show(int flag, float tint_percent) {
	current->draw(x, y, flag, tint_percent);
}
//show the rocket
void Rocket::effect(Player *player) {
	x = player->get_x() - 7 * 3;
	y = player->get_y();
	player->set_dy(-20);
	if (player->attach != NULL) {
		delete player->attach;
		player->attach = NULL;
	}
	player->attach = this;
	player->current = player->jumping;
	timer = new Timer(2);
	current = subordinate;
}//set the effect for rocket

bool Rocket::allow(Player *player) {
	return true;
}

bool Rocket::collide(Player *player, int max_width) {
	float x2, y2, xp, yp, w, h, wp, hp;
	x2 = player->get_x() + player->get_dx();
	if (player->get_flag() == ALLEGRO_FLIP_HORIZONTAL) x2 += player->get_width() / 3;
	y2 = player->get_y() + player->get_dy();
	w = player->get_width() * 2 / 3;
	h = player->get_height();
	xp = x;
	yp = y;
	wp = get_width();
	hp = get_height();
	bool x_collision = (x2 < xp + wp && x2 + w > xp);
	bool y_collision = (y2 < yp + hp && y2 + h >= yp);
	if (player->get_x() + player->get_width() > max_width) x_collision = (x_collision || x2 - max_width < xp + wp && x2 - max_width + w > xp);
	else if (player->get_x() < 0) x_collision = (x_collision || x2 + max_width < xp + wp && x2 + max_width + w > xp);
	return x_collision && y_collision;
}//checking for doole collide to rocket

bool Rocket::time_out() {
	return (timer == NULL || timer->passed());
}