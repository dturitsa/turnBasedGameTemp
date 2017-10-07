#pragma once
#include "PhysicsEngine.h"

class PhysicsSystem : public PhysicsEngine
{
public:
	PhysicsObject Wind;

	PhysicsSystem();
	~PhysicsSystem();
	void StartPhysicsLoop();
	//void PhysicsSystem::handleMessage(Msg *msg);
	void setWind(float angle, float speed);
	void changeMast(std::string ID, int mast);
	void changeRudder(std::string ID, int rudder);
	void updateMovement(PhysicsObject &object);
};
