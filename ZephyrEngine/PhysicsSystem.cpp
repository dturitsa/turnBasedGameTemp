#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem(MessageBus* mbus) : System (mbus)
{
	//create wind object(move to message handle?)
	Physics.addObject("Wind", "Wind", 0, 0, 0, 0, 0, 1, 0);
}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::StartPhysicsLoop()
{
	/*
	clock_t	thisTime = clock();
	clock_t lastTime = thisTime;

	while (true)
	{
		thisTime = clock();
		if ((thisTime - lastTime) > 100)
		{
			lastTime = thisTime;

			//FOR FUTURE REFERENCE: Loop through Physics objects to update their movements and check collision
			for (std::map<std::string, PhysicsObject>::iterator it = GameObjects.begin(); it != GameObjects.end(); ++it)
			{
				updateMovement(it->second);
				//checkCollision
			}
		}
	}
	*/
}

/*
void PhysicsSystem::handleMessage(Msg *msg)
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

//Updates game object movement. Determines movement by tag(ship, projectile, etc...).
void PhysicsSystem::updateMovement(PhysicsObject &object)
{
	if (object.tag == "Ship")
	{
		Vector2 objectDirection;
		float windPercentage;
		float movementScale = 0;

		switch (object.rudder)
		{
		case STRAIGHT:
			break;
		case HALFPORT:
			object.rotation -= object.rotationSpeed / 2;
			break;
		case FULLPORT:
			object.rotation -= object.rotationSpeed;
			break;
		case HALFSTARBOARD:
			object.rotation += object.rotationSpeed / 2;
			break;
		case FULLSTARBOARD:
			object.rotation += object.rotationSpeed;
			break;
		}
		object.rotation = Physics.checkAngle(object.rotation);

		switch (object.mast)
		{
		case NONE:
			movementScale = 0;
			break;
		case HALFMAST:
			movementScale = (object.windScale * Physics.GameObjects["Wind"].windScale) / 2;
			break;
		case FULLMAST:
			movementScale = object.windScale * Physics.GameObjects["Wind"].windScale;
			break;
		}

		windPercentage = Physics.GameObjects["Wind"].rotation - object.rotation;
		Physics.absolute(windPercentage);
		windPercentage = 1 - (windPercentage / 180);
		Physics.absolute(windPercentage);
		movementScale *= windPercentage;
		objectDirection = Physics.convertAngleToVector(object.rotation);
		object.position.translate(objectDirection.x * movementScale, objectDirection.y * movementScale);
	}

	else if (object.tag == "Projectile")
	{

	}
}