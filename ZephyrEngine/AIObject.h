#pragma once
#include <string>
#include <windows.h>
#include <math.h>
//#include "AISystem.h"
#include "AIData.h"


struct vector2
{
	float x;
	float y;
};

//objects in the world that are of interest to the AI system
class WorldObject
{
public:
	std::string id;
	int orientation;
	vector2 pos;
};



class AIObject
{
public:
	AIObject();
	~AIObject();
	void update();
	int angleToTarget(vector2 origin, vector2 target);
	int distanceToTarget(vector2 origin, vector2 target);
	void turnToFace(int newOrientation);

	std::string id;
	int orientation;
	vector2 pos;
	AIData* aiData;
	WorldObject* target;

};



