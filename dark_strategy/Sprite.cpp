#include "Sprite.h"
#include <iostream>


Sprite::Sprite(const char *filename, int quantity, double delay)
{
	image = al_load_bitmap(filename);
	this->quantity = quantity;

	width = al_get_bitmap_width(image);
	height = al_get_bitmap_height(image) / quantity;
	timer = new Timer(delay);
}
//loading image
Sprite::Sprite(ALLEGRO_BITMAP *img, int quantity, double delay)
{
	image = img;
	this->quantity = quantity;

	width = al_get_bitmap_width(image);
	height = al_get_bitmap_height(image) / quantity;
	timer = new Timer(delay);
}
//play the image with animation

Sprite::~Sprite()
{
	delete timer; 
	
}

void Sprite::draw(float x, float y, int flag, float tint_percent){
	al_draw_tinted_bitmap_region(image, al_map_rgba_f(1, 1, 1, 1 - tint_percent), 0, height * current, width, height, x, y, flag);
	if (timer->passed() == true) {
		next();
		timer->reset();
	}
}
//draw the image at the postion
void Sprite::next() {
	if (allow || current != quantity - 1) current = (current + 1) % quantity;
}
//check for the current image to load the next or loop
int Sprite::get_height() {
	return height;
}

int Sprite::get_width() {
	return width;
}

int Sprite::get_quantity() {
	return quantity;
}

void Sprite::get_current_of(Sprite *other) {
	current = other->current;
}