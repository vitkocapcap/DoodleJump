#pragma once
class Platform;
class Object;

class Character
{
protected:
	float x, y, dx, dy;
public:
	friend Platform;
	friend Object;

	virtual void show(float tint_percent, float max_width) = 0;
	virtual void update(float gravity) = 0;
};