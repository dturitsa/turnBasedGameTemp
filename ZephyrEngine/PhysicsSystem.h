#pragma once
#include "System.h"

class PhysicsSystem
{
public:
	PhysicsSystem(MessageBus* mbus);
	~PhysicsSystem();
	void addGameObject();
	void removeGameObject();
	void handleMessage(Msg *msg);
	void calculatePosition();
	void updateGameObject();
	void checkCollision();
};

