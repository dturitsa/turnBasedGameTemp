#pragma once
#include <string>
#include <windows.h>
#include <math.h>
//#include "AISystem.h"
#include "AIData.h"
#include "AIDNA.h"
#include "Vector2.h"

//objects in the world that are of interest to the AI system
class WorldObject
{
public:
	std::string id;
	int orientation;
	Vector2 pos;
	int height;
	int width;
	Vector2 c[4]; //corner positions
};



class AIObject
{
public:
	AIObject();
	~AIObject();
	void update();
	int angleToTarget(Vector2 origin, Vector2 target);
	int distanceToTarget(Vector2 origin, Vector2 target);
	int collisionTimer;
	int timeAlive = 0;


	void turnToFace(int newOrientation);
	int seekBehaviour();
	int engageBehaviour();
	float colAvoidanceBehaviour();
	float checkAngle(float angle);
	float checkIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d);
	float checkCollision();
	inline int signedOrientation(int unsignedOrientation);
	inline int angleBetween(Vector2 v1, Vector2 v2);
	void scoredHit(std::string hitObjectId, std::string projectileId); //object hit something

	std::string id;
	int orientation;
	Vector2 pos;
	AIData* aiData;
	AIDNA* dna;
	WorldObject* target;
	int cannonCooldownCounter = 0;

};



