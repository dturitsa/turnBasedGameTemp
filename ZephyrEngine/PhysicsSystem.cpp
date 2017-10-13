#include "PhysicsSystem.h"

/*
HANK

There are issues with the messages not having enough information. Discussed this with Denis and he has a solution we can implement after alpha.
Have a look at handleMessage function. There are a lot of uncertainties with the messages, so I figured you should handle it for testing purposes.
Check the functions changeMast and changeRudder as well, and change it however you want for your tests.

You can use the StartPhysicsLoop for your tests. It just iterates through the PhysicsObject map, and updates ships and projectiles.
The tags for now seems to be the renderable string as discussed with Denis. So maybe name the image files Ship.png and Projectile.png, then change the loop to look for tag == "Ship.png" and stuff.

This class is basically the bridge between PhysicsEngine and the actual Game stuff, so we really should've worked on it together.
I have all the functions to do everything set up, so just use them with your messages for testing.
*/

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

//Subject to change! Temporary solution for testing and alpha build.
//Parses the msg string, then switch case msg type.
void PhysicsSystem::handleMessage(Msg *msg)
{
	std::vector<std::string> data;
	std::string token;
	std::string messageData = msg->data;
	std::string splitter = ", ";
	std::size_t pos = 0;
	std::string ID, tag;
	float x, y, rotation, width, height;

	while ((pos = messageData.find(splitter)) != std::string::npos)
	{
		token = messageData.substr(0, pos);
		data.push_back(token);
	}

	ID = data[0];
	tag = data[1];
	x = data[2];
	y = data[3];
	//skip z
	rotation = data[5];
	width = 6;//data[6]?
	height = 7;//data[7]?

	switch (msg->type)
	{
	case GO_ADDED:
		//Subject to change! Need to finalize message data system to identify the object type.
		Physics.addObject(ID, tag, x, y, width, height, rotation, 1, 1, PROJECTILE_INERTIA);
		break;
	case GO_REMOVED:
		Physics.removeObject(ID);
		break;
		/*
		//These are examples
		case CHANGE_MAST:
		changeMast(ID, mast); //need to add in the string parser above
		break;
		case CHANGE_RUDDER:
		changeRudder(ID, rudder); //need to add in the string parser above
		break;
		*/
	}
}

void PhysicsSystem::setWind(float angle, float speed)
{
	Physics.GameObjects["Wind"].rotation = Physics.checkAngle(angle);
	Physics.GameObjects["Wind"].windScale = speed;
}

void PhysicsSystem::changeMast(std::string ID, int mast)
{
	//Use this if you implement enum mast to the message. Change int mast to Mast mast in the parameters above.
	//Physics.GameObjects[ID].mast = mast;

	//Otherwise you can test using an int
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
	//Use this if you implement enum mast to the message. Change int rudder to Rudder rudder in the parameters above.
	//Physics.GameObjects[ID].rudder = rudder;

	//Otherwise you can test using an int
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