#include "Timer.h"


Timer::Timer(double max_time)
{
	checkpoint = al_get_time();
	
	this->max_time = max_time;
}
//max_time is the delay time we use after

Timer::~Timer()
{
}

void Timer::reset() {
	checkpoint = al_get_time();
}
//get the local time
bool Timer::passed() {
	return al_get_time() - checkpoint > max_time;
}//checking whether time passed or not