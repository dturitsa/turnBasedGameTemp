#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(std::string tagIn, Vector2 posIn, float wIn, float hIn, float rotIn, float scale, float rotSpeedIn)
{
	this->tag = tagIn;
	this->position = posIn;
	this->width = wIn;
	this->height = hIn;
	this->rotation = rotIn;
	this->mast = NONE;
	this->rudder = STRAIGHT;
	this->windScale = scale;
	this->rotationSpeed = rotSpeedIn;
}

PhysicsObject::~PhysicsObject()
{
}
