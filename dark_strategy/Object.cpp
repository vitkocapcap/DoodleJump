#include "Object.h"


Object::Object()
{
}


Object::~Object()
{
	delete standard;
	delete subordinate;
}


int Object::get_width() {
	return current->get_width();
}
int Object::get_height() {
	return current->get_height();
}

void Object::set_audio(ALLEGRO_SAMPLE *_sound) {
	sound = _sound;
}//add sound