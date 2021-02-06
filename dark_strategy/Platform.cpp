#include "Platform.h"
#include <iostream>

Platform::~Platform()
{
	delete standard;
	if (subordinate != NULL) delete subordinate;
	if (attach != NULL) delete attach;
}

void Platform::show(float tint_percent) {
	if (attach != NULL) attach->show(flag, tint_percent);
	current->draw(x, y, flag, tint_percent + self_tint);
	if (self_tint < 1) self_tint += self_tint / 10;
	if (self_tint > 1) self_tint = 1;
}//show platform an setup tint effect

void Platform::update(float offset, float width_limit) {
	y += offset;
	x += dx;
	if (attach != NULL) {
		attach->y += offset;
		attach->x += dx;
	}
	int w = get_width();
	if (x + w >= width_limit) {
		x = width_limit - w;
		dx *= -1;
	}
	else if (x < 0) {
		x = 0;
		dx *= -1;
	}
}//update the postion to move the platform when doodle jump higher and higher

void Platform::set_flag(int _flag) {
	flag = _flag;
}

void Platform::set_code(int _code) {
	code = _code;
}

void Platform::set_location(float _x, float _y) {
	x = _x;
	y = _y;
}

void Platform::set_dx(float _dx) {
	dx = _dx;
}

void Platform::set_x(float _x) {
	x = _x;
}

int Platform::get_width() {
	return standard->get_width();
}

int Platform::get_height() {
	return standard->get_height();
}

float Platform::get_x() {
	return x;
}

bool Platform::out_of_sight(int max_height) {
	return (y > max_height) && (attach == NULL || attach->y > max_height);
}
//checking platform out of sight
bool Platform::to_be_destroyed() {
	return end || self_tint == 1;
}//boolean to allow to delete platform out of sight

bool Platform::collide(Player *player, int max_width) {
	float x2, y2, xp, yp, w, h, wp, hp;
	x2 = player->x + player->dx;
	if (player->flag == ALLEGRO_FLIP_HORIZONTAL) x2 += player->get_width() / 3;
	y2 = player->y + player->dy;
	w = player->get_width() * 2 / 3;
	h = player->get_height();
	xp = x;
	yp = y;
	wp = get_width();
	hp = get_height();
	bool x_collision = (x2 < xp + wp && x2 + w > xp);
	bool y_pre_collision = player->y + h < yp;
	bool y_collision = y2 + h >= yp;
	if (player->x + player->current->get_width() > max_width) x_collision = (x_collision || x2 - max_width < xp + wp && x2 - max_width + w > xp);
	else if (player->x < 0) x_collision = (x_collision || x2 + max_width < xp + wp && x2 + max_width + w > xp);
	return x_collision && y_pre_collision && y_collision;
}
//checking for colliding between player and platform
bool Platform::collide_attach(Player *player, int max_width) {
	if (attach == NULL) return false;
	else return attach->collide(player, max_width);
}

void Platform::fix_as(Player *player) {
	player->y = y - player->get_height();
}

void Platform::set_attach(Object *obj) {
	obj->x = x + 5 + rand() % int(get_width() - 5 - obj->get_width());
	obj->y = y - obj->get_height();
	attach = obj;
}
//checking for object on platform
void Platform::attach_affect(Player *player) {
	attach->effect(player);
	if (attach->trans == true) attach = NULL;
}//release the effect

bool Platform::attach_allow(Player *player) {
	if (attach != NULL) return attach->allow(player);
	else return false;
}