#pragma once
#include "PhysicsHeader.h"
#include "Vector2.h"
class Axis
{
public:
	Vector2 axis;
	Vector2 Acorners[4];
	Vector2 Bcorners[4];
	Vector2 Amin, Amax, Bmin, Bmax;
	Axis();
};

