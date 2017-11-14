#pragma once
#include <string>
#include <windows.h>
//#include "AISystem.h"
#include "AIData.h"

//objects in the world that are of interest to the AI system
class WorldObject
{
public:
	std::string id;
	int x, y, orientation;
};

class AIObject
{
public:
	AIObject();
	~AIObject();
	void update();
	std::string id;
	int x, y, orientation;
	AIData* aiData;
	WorldObject* target;

};



