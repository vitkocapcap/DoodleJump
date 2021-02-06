#pragma once
#include <allegro5\allegro.h>

class Timer
{
private:
	double checkpoint, max_time;
public:
	Timer(double max_time);
	~Timer();

	void reset();
	bool passed();
};

