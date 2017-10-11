#pragma once
#include "PhysicsHeader.h"
#include "PhysicsObject.h"
#include "Vector2.h"

class PhysicsEngine
{
public:
	std::map<std::string, PhysicsObject> GameObjects;

	PhysicsEngine();
	~PhysicsEngine();

	void addObject(std::string ID, std::string tag, float x, float y, float width, float height, float rotation, float windscale, float rotationspeed, float inertia);
	void removeObject(std::string ID);
	void absolute(float &number);
	Vector2 PhysicsEngine::convertAngleToVector(float angle);
	float PhysicsEngine::convertVectorToAngle(Vector2 vector);
	float checkAngle(float angle);
	bool collisionCheck(PhysicsObject o1, PhysicsObject o2);
	void getBox(PhysicsObject o, Vector2(&box)[4]);
};
