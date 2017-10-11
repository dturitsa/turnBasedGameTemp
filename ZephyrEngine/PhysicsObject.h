#pragma once
#include "PhysicsHeader.h"
#include "Vector2.h"

class PhysicsObject
{
public:
	std::string tag;
	Vector2 position;
	float width;
	float height;
	float rotation;
	Mast mast;
	Rudder rudder;
	float windScale;
	float rotationSpeed;
	float inertia;

	PhysicsObject();
	PhysicsObject(std::string tagIn, float xIn, float yIn, float wIn, float hIn, float rotIn, float scale, float rotSpeedIn, float inertiaIn);
	~PhysicsObject();
};

