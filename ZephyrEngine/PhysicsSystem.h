#pragma once
#include "PhysicsEngine.h"
#include "System.h"

#define PROJECTILE_FORCE 4
#define PROJECTILE_INERTIA 100

class PhysicsSystem : public System
{
public:
	PhysicsEngine Physics;

	PhysicsSystem(MessageBus* mbus);
	~PhysicsSystem();

	void startSystemLoop();
	void handleMessage(Msg *msg);

	void setWind(float angle, float speed);
	void changeMast(std::string ID, int mast);
	void changeRudder(std::string ID, int rudder);
	void updateShip(PhysicsObject &ship);
	void updateProjectile(PhysicsObject &projectile);
	void Destroy(PhysicsObject &object);

	int timeFrame = 10;


};
