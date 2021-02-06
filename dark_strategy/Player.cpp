#include "Player.h"
#include "Object.h"
#include <iostream>

unsigned convert(const char * str) {
	unsigned ans = 0;
	for (int idx = 0; str[idx] != '\0'; idx++) ans = ans * 10 + (str[idx] - '0');
	return ans;
}//convert the config to number

Player::Player(ALLEGRO_CONFIG *cfg, double delay)
{

	attach = NULL;

	const char *jmp, *fll;
	jmp = al_get_config_value(cfg, "", "jumping");
	fll = al_get_config_value(cfg, "", "falling");
	
	//get the config value of image
	int jmp_quann, fll_quann;

	jmp_quann = convert(al_get_config_value(cfg, "", "jumping quantity"));
	fll_quann = convert(al_get_config_value(cfg, "", "falling quantity"));
	
	//change to quantity, at this is just one, but can be changed easily

	jumping = new Sprite(jmp, jmp_quann, delay);
	falling = new Sprite(fll, fll_quann, delay);
	//create sprite

	current = falling;
}


Player::~Player()
{
	delete jumping;
	delete falling;


	if (attach != NULL) delete attach;
}

void Player::show(float tint_percent, float max_width) {
	if (x > max_width) x -= max_width;
	if (x + current->get_width() < 0) x += max_width;
	if (attach != NULL) {
		if (attach->x > max_width) attach->x -= max_width;
		if (attach->x + attach->get_width() < 0) attach->x += max_width;
	}//checking attach to set x
	current->draw(x, y, flag, tint_percent);
	if (attach != NULL) attach->show(flag, tint_percent);
	if (x + current->get_width() > max_width) {
		current->draw(x - max_width, y, flag, tint_percent);
		if (attach != NULL) {
			attach->x -= max_width;
			attach->show(flag, tint_percent);
			attach->x += max_width;
		}
	}//checking attach to draw 
	else if (x < 0) {
		current->draw(x + max_width, y, flag, tint_percent);
		if (attach != NULL) {
			attach->x += max_width;
			attach->show(flag, tint_percent);
			attach->x -= max_width;
		}
	}//draw player
}

void Player::update(float gravity) {
	x += dx;
	y += dy;
	if (attach != NULL) {
		attach->x += dx;
		attach->y += dy;
		if (attach->time_out()) {
			delete attach;
			attach = NULL;
		}
		else return;
	}
	if (dy < 0 && dy + gravity >= 0) current = falling;
	dy += gravity;
}//update the postion, velocity and gravity

int Player::get_width() {
	return current->get_width();
}

int Player::get_height() {
	return current->get_height();
}

float Player::get_y() {
	return y;
}

float Player::get_x() {
	return x;
}

float Player::get_dx() {
	return dx;
}

bool Player::is_falling() {
	return current == falling;
}

void Player::set_y(float _y) {
	if (attach != NULL) attach->y -= (y - _y);
	y = _y;
}

void Player::set_location(float _x, float _y) {
	x = _x, y = _y;
}

void Player::set_dy(float _dy) {
	dy = _dy;
}

void Player::set_dx(float _dx) {
	dx = _dx;
}

void Player::set_jumping() {
	current = jumping;
}

bool Player::die(int max_height){
	return y > max_height;
}

void Player::set_flag(int _flag){
	flag = _flag;
}

int Player::get_flag() {
	return flag;
}


float Player::get_dy() {
	return dy;
}