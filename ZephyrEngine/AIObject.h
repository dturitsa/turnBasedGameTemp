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
	int height;
	int width;
	vector2 c[4]; //corner positions
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
	int seekBehaviour();
	int engageBehaviour();
	int colAvoidanceBehaviour();
	inline int signedOrientation(int unsignedOrientation);
	inline int angleBetween(vector2 v1, vector2 v2);

	std::string id;
	int orientation;
	vector2 pos;
	AIData* aiData;
	WorldObject* target;

};



