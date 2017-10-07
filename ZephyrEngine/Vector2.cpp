#include "Vector2.h"

Vector2::Vector2()
{
}

Vector2::Vector2(float xIn, float yIn)
{
	this->x = xIn;
	this->y = yIn;
}

void Vector2::normalize()
{
	float xFinal;
	float yFinal;
	float v;
	v = sqrt((this->x * this->x) + (this->y * this->y));
	xFinal = this->x / v;
	yFinal = this->y / v;
	this->x = xFinal;
	this->y = yFinal;
}

void Vector2::translate(float xOffset, float yOffset)
{
	this->x += xOffset;
	this->y += yOffset;
}

void Vector2::rotate(float angle)
{
	float radians;
	float xTemp, yTemp;
	radians = angle * -1 * (PI / 180);
	xTemp = this->x;
	yTemp = this->y;
	this->x = xTemp * cos(radians) + yTemp * -sin(radians);
	this->y = xTemp * sin(radians) + yTemp * cos(radians);
}

void Vector2::rotateFromOrigin(Vector2 origin, float angle)
{
	float xOffset;
	float yOffset;
	xOffset = origin.x * -1;
	yOffset = origin.y * -1;
	this->translate(xOffset, yOffset);
	this->rotate(angle);
	this->translate(origin.x, origin.y);
}
