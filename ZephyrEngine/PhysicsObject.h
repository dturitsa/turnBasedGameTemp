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

	PhysicsObject();
	PhysicsObject(std::string tagIn, Vector2 posIn, float wIn, float hIn, float rotIn, float scale, float rotSpeedIn);
	~PhysicsObject();
};

