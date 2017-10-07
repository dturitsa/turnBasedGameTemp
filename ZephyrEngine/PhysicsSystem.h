#pragma once
#include "PhysicsEngine.h"
#include "System.h"

class PhysicsSystem : public System
{
public:
	PhysicsEngine Physics;
	PhysicsSystem(MessageBus* mbus);
	~PhysicsSystem();
	void StartPhysicsLoop();
	//void PhysicsSystem::handleMessage(Msg *msg);
	void setWind(float angle, float speed);
	void changeMast(std::string ID, int mast);
	void changeRudder(std::string ID, int rudder);
	void updateMovement(PhysicsObject &object);
};
