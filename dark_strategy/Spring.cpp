#include "Spring.h"

Spring::Spring(ALLEGRO_BITMAP *img, ALLEGRO_BITMAP *img2) {
	current = standard = new Sprite(img, 1, 1);
	subordinate = new Sprite(img2, 1, 1);
	trans = false;
	timer = NULL;
}//load images for spring before and after collide


void Spring::show(int flag, float tint_percent) {
	current->draw(x, y, flag, tint_percent);
}
//draw spring
bool Spring::allow(Player *player) {
	return player->get_dy() >= 0;
}

void Spring::effect(Player *player) {
	player->set_dy(-30);
	player->set_y(y - player->get_height());
	y -= 3 * 3;
	al_play_sample(sound, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
	current = subordinate;
}//add the effect for spring as well as sound
//however, from the spring on early platform often happens error

bool Spring::collide(Player *player, int max_width) {
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
	bool y_pre_collision = player->get_y() + h < yp;
	bool y_collision = y2 + h >= yp;
	if (player->get_x() + player->get_width() > max_width) x_collision = (x_collision || x2 - max_width < xp + wp && x2 - max_width + w > xp);
	else if (player->get_x() < 0) x_collision = (x_collision || x2 + max_width < xp + wp && x2 + max_width + w > xp);
	return x_collision && y_pre_collision && y_collision;
}//checking for doodle collide to spring or not

bool Spring::time_out() {
	return true;
}