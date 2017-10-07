#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::addObject(std::string ID, std::string tag, float xPosition, float yPosition, float width, float height, float rotation, float windscale, float rotationspeed)
{
	PhysicsObject NewObject(tag, xPosition, yPosition, width, height, rotation, windscale, rotationspeed);
	GameObjects[ID] = NewObject;
}

void PhysicsEngine::removeObject(std::string ID)
{
	GameObjects.erase(ID);
}

void PhysicsEngine::absolute(float &number)
{
	number = sqrt(number * number);
}

Vector2 PhysicsEngine::convertAngleToVector(float angle)
{
	Vector2 result;
	result.x = 0;
	result.y = 1;
	result.rotate(checkAngle(angle));
	return result;
}

float PhysicsEngine::convertVectorToAngle(Vector2 vector)
{
	float result;
	if (vector.x >= 0 && vector.y >= 0)
		result = atan(abs(vector.x) / vector.y);
	else if (vector.x >= 0 && vector.y <= 0)
		result = 90 + atan(abs(vector.y) / abs(vector.x));
	else if (vector.x <= 0 && vector.y <= 0)
		result = 180 + atan(vector.x / abs(vector.y));
	else if (vector.x <= 0 && vector.y >= 0)
		result = 270 + atan(vector.y / vector.x);
	if (vector.x == 0 && vector.y == 0)
		result = NULL;

	return result;
}

//ensures angle is between 0 and 360
float PhysicsEngine::checkAngle(float angle)
{
	float result;
	result = angle;
	if (result < 0 || result > 360)
	{
		while (result > 360)
		{
			result -= 360;
		}
		while (result < 0)
		{
			result += 360;
		}
	}
	return result;
}

//collision
bool PhysicsEngine::collisionCheck(PhysicsObject o1, PhysicsObject o2)
{
	Vector2 box1[4], box2[4];
	return true;
}

//collision
void PhysicsEngine::getBox(PhysicsObject o, Vector2(&box)[4]) {

	float sinAngle = sin(o.rotation * PI / 180);
	float cosAngle = cos(o.rotation * PI / 180);
	float halfWidth = o.width / 2;
	float halfHeight = o.height / 2;
	// [0] = Upper Left
	box[0].x = o.position.x - halfWidth;
	box[0].y = o.position.y + halfHeight;
	box[0].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [1] = Upper Right
	box[1].x = o.position.x + halfWidth;
	box[1].y = o.position.y + halfHeight;
	box[1].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [2] = Bottom Right
	box[2].x = o.position.x + halfWidth;
	box[2].y = o.position.y - halfHeight;
	box[2].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));

	// [3] = Bottom Left
	box[3].x = o.position.x - halfWidth;
	box[3].y = o.position.y - halfHeight;
	box[3].rotateFromOrigin(o.position.x, o.position.y, checkAngle(o.rotation));
}
