#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(MessageBus* mbus) : System (mbus)
{
	//create wind object(move to message handle?)
	Physics.addObject("Wind", "Wind", 0, 0, 0, 0, 0, 1, 0, 0);
}

PhysicsSystem::~PhysicsSystem()
{
}

//Subject to change! This was used for testing.
void PhysicsSystem::StartPhysicsLoop()
{
	clock_t	thisTime = clock();
	clock_t lastTime = thisTime;

	while (true)
	{
		thisTime = clock();
		if ((thisTime - lastTime) > 100)
		{
			lastTime = thisTime;

			//Subject to change! Will discuss how the loop will actually work.
			//Loop through Physics objects to update their movements and check collision.
			for (std::map<std::string, PhysicsObject>::iterator it = Physics.GameObjects.begin(); it != Physics.GameObjects.end(); ++it)
			{
				if (it->second.tag == "Ship")
				{
					updateShip(it->second);
				}
				else if (it->second.tag == "Projectile")
				{
					updateProjectile(it->second);
				}
				else if (it->second.tag == "Remove")
				{
					it = Physics.GameObjects.erase(it);
				}
				//checkCollision
			}
		}
	}
}

/*
Need to know message stuff to implement these

void PhysicsSystem::handleMessage(Msg *msg)
{

}

void PhysicsSystem::createShip()
{

}

void PhysicsSystem::createProjectile()
{

}
*/

void PhysicsSystem::setWind(float angle, float speed)
{
	Physics.GameObjects["Wind"].rotation = Physics.checkAngle(angle);
	Physics.GameObjects["Wind"].windScale = speed;
}

void PhysicsSystem::changeMast(std::string ID, int mast)
{
	switch (mast)
	{
	case 0:
		Physics.GameObjects[ID].mast = NONE;
		break;
	case 1:
		Physics.GameObjects[ID].mast = HALFMAST;
		break;
	case 2:
		Physics.GameObjects[ID].mast = FULLMAST;
		break;
	}
}

void PhysicsSystem::changeRudder(std::string ID, int rudder)
{
	switch (rudder)
	{
	case 0:
		Physics.GameObjects[ID].rudder = STRAIGHT;
		break;
	case 1:
		Physics.GameObjects[ID].rudder = HALFSTARBOARD;
		break;
	case 2:
		Physics.GameObjects[ID].rudder = FULLSTARBOARD;
		break;
	case 3:
		Physics.GameObjects[ID].rudder = HALFPORT;
		break;
	case 4:
		Physics.GameObjects[ID].rudder = FULLPORT;
		break;
	}
}

//Updates ship movement using its mast and rudder settings.
//The speed is determined by the wind power scaled by the mast setting: halfsail = 50%, fullsail = 100%
//Then it is further scaled by comparing the ship direction and the wind direction (percentage based).
//FUTURE REFERENCE: When creating different classes of ships, apply the object's windFactor to have different speeds.
void PhysicsSystem::updateShip(PhysicsObject &ship)
{
	Vector2 objectDirection;
	float windPercentage;
	float movementScale = 0;

	switch (ship.rudder)
	{
	case STRAIGHT:
		break;
	case HALFPORT:
		ship.rotation -= ship.rotationSpeed / 2;
		break;
	case FULLPORT:
		ship.rotation -= ship.rotationSpeed;
		break;
	case HALFSTARBOARD:
		ship.rotation += ship.rotationSpeed / 2;
		break;
	case FULLSTARBOARD:
		ship.rotation += ship.rotationSpeed;
		break;
	}
	ship.rotation = Physics.checkAngle(ship.rotation);

	switch (ship.mast)
	{
	case NONE:
		movementScale = 0;
		break;
	case HALFMAST:
		movementScale = (ship.windScale * Physics.GameObjects["Wind"].windScale) / 2;
		break;
	case FULLMAST:
		movementScale = ship.windScale * Physics.GameObjects["Wind"].windScale;
		break;
	}

	windPercentage = Physics.GameObjects["Wind"].rotation - ship.rotation;
	Physics.absolute(windPercentage);
	windPercentage = 1 - (windPercentage / 180);
	Physics.absolute(windPercentage);
	movementScale *= windPercentage;
	objectDirection = Physics.convertAngleToVector(ship.rotation);
	ship.position.translate(objectDirection.x * movementScale, objectDirection.y * movementScale);
}

//Simple shooting for alpha. Projectile slows a bit overtime.
void PhysicsSystem::updateProjectile(PhysicsObject &projectile)
{
	if (projectile.inertia > (PROJECTILE_INERTIA - 30))
	{
		Vector2 objectDirection;
		float forceScale, force;

		objectDirection = Physics.convertAngleToVector(projectile.rotation);
		forceScale = projectile.inertia / 100;
		force = PROJECTILE_FORCE * forceScale;

		projectile.position.translate(objectDirection.x * force, objectDirection.y * force);
		projectile.inertia--;
	}
	else
	{
		Destroy(projectile);
	}
}

//Subject to change! Depends on how we loop through the GameObjects.
//Might need to use this to safely remove objects, if we are using iterator to loop through the GameObjects map.
void PhysicsSystem::Destroy(PhysicsObject &object)
{
	object.tag = "Remove";
}