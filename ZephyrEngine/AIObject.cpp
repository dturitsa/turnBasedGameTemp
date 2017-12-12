#include "AIObject.h"

using namespace std;

AIObject::AIObject()
{
	target = NULL;
	collisionTimer = 999;
}

AIObject::~AIObject()
{
}

void AIObject::update() {
	//set mast
	string msgData = id + ",2,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

	msgData = id + ",1,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));

	//msgData = id + ",2,Boat_S2.png";
	//aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));

	collisionTimer++;
	//set the target
	//if (target == NULL) {

		WorldObject* closestEnemy = aiData->worldObjects[0];
		float closestEnemyDist = 999999;

		//set target to player
		for (WorldObject* w : aiData->worldObjects) {
			////set target to player
			//if (w->id == "playerShip") {
			//	target = w;
			//}

			//set target to closest AI object (for AI training)
			float enemyDist = sqrt(pow(w->pos.x - pos.x, 2) + pow(w->pos.y - pos.y, 2));
			if (enemyDist < closestEnemyDist && (w->id.find("enemy") != string::npos) && w->id != id) {
				closestEnemy = w;
				closestEnemyDist = enemyDist;
			}
		}
		if (closestEnemy != target) {
			target = closestEnemy;
			OutputDebugString("\n");
			OutputDebugString(id.c_str());
			OutputDebugString(" Set target to ");
			OutputDebugString(target->id.c_str());
			OutputDebugString("\n");
		}


		//set target to closest "enemy"

	//}
	if (colAvoidanceBehaviour() != 0)
		collisionTimer = 0;

	if (collisionTimer > 40 && target != NULL) {
		int range = distanceToTarget(pos, target->pos);

		if (range > 90) {
			seekBehaviour();
		}
		else if (range < 80) {
			engageBehaviour();
		}
	}
}

//ensures angle is between 0 and 360
float AIObject::checkAngle(float angle)
{
	float result;
	result = angle;
	if (result < 0 || result > 360)
	{
		while (result > 360)
		{
			result -= 360;
		}
		while (result < 0)
		{
			result += 360;
		}
	}
	return result;
}


bool onSegment(Vector2 p, Vector2 q, Vector2 r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}



float AIObject::checkIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
	float x1 = a.x; float y1 = a.y;
	float x2 = b.x; float y2 = b.y;
	float x3 = c.x; float y3 = c.y;
	float x4 = d.x; float y4 = d.y;
	float xNumerator = (x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4);
	float xDenominator = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
	float yNumerator = (x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4);
	float yDenominator = (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4);
	if (xDenominator == 0 || yDenominator == 0)
		return 0; // there was no collision
	float x = xNumerator / xDenominator;
	float y = yNumerator / yDenominator;


	float distanceAP = sqrt(pow(x - x1, 2) + pow(y - y1, 2));
	float distanceAB = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	if (distanceAB < distanceAP) // checking if there was a collision
		return 0; // there was no collision

	float distanceCP = sqrt(pow(x3 - x, 2) + pow(y3 - y, 2));
	float distanceDP = sqrt(pow(x4 - x, 2) + pow(y4 - y, 2));
	float distanceCD = sqrt(pow(x4 - x3, 2) + pow(y4 - y3, 2));

	if (abs(distanceCP + distanceDP - distanceCD) < .01) {
		return distanceAP;
	}


	return 0;
}

int AIObject::colAvoidanceBehaviour() {
	float collisionDistance = checkCollision();


	string msgData = id + ",0,Boat_S2.png";

	if (collisionDistance > 0) {
		msgData = id + ",0,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
		OutputDebugString("AVOIDING COLLISION");
		OutputDebugString("\n");
	}
	else if (collisionDistance < 0) {
		msgData = id + ",4,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
	}
	else if (collisionDistance == 0 && collisionTimer > 20) {
		msgData = id + ",2,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
	}

	return collisionDistance;
}

int AIObject::checkCollision() {
	string collidedObject = "nothing";
	float raycastLength = 100;
	float raycastAngle = 10;
	Vector2 leftRaycast(pos.x, pos.y + raycastLength);
	Vector2 rightRaycast(pos.x, pos.y + raycastLength);
	float leftAngle = checkAngle(orientation - raycastAngle);
	float rightAngle = checkAngle(orientation + raycastAngle);
	leftRaycast.rotateFromOrigin(pos.x, pos.y, leftAngle);
	rightRaycast.rotateFromOrigin(pos.x, pos.y, rightAngle);
	float leftColDistance = 9999;
	float rightColDistance = 9999;


	for (WorldObject* w : aiData->worldObjects) {

		if (w->id != "island1" && w->id != "invisibleWallWest" && w->id != "invisibleWallEast"
			&& w->id != "invisibleWallNorth" && w->id != "invisibleWallSouth")
			continue;

		for (int j = 0; j < 4; j++) {
			float colDistance = checkIntersection(pos, leftRaycast, w->c[j], w->c[(j + 1) % 4]);


			if (colDistance < leftColDistance && colDistance != 0) {
				leftColDistance = colDistance;
				collidedObject = w->id;

			}

			colDistance = checkIntersection(pos, rightRaycast, w->c[j], w->c[(j + 1) % 4]);
			if (colDistance < rightColDistance && colDistance != 0) {
				rightColDistance = colDistance;
				collidedObject = w->id;
			}
		}
		//OutputDebugString("\n");
	}

	if (leftColDistance == 9999)
		leftColDistance = 0;
	if (rightColDistance == 9999)
		rightColDistance = 0;

	//OutputDebugString(collidedObject.c_str());

	if (leftColDistance != 0 && leftColDistance < rightColDistance)
		return -leftColDistance;
	else if (rightColDistance != 0)
		return rightColDistance;

	return 0;
}
inline int AIObject::angleBetween(Vector2 v1, Vector2 v2) {
	float dot = v1.x*v2.x + v1.y*v2.y;     // dot product between[x1, y1] and [x2, y2]
	float det = v1.x*v2.y - v1.y*v2.x;     // determinant
	return atan2(det, dot);
}



//close distance to the traget the target
int AIObject::seekBehaviour() {
	int faceAngle = angleToTarget(this->pos, target->pos);
	turnToFace(faceAngle);

	return faceAngle;
}

//aim guns on target and fire
int AIObject::engageBehaviour() {
	int faceAngle = angleToTarget(this->pos, target->pos);
	//int faceAngle = target->orientation;
	int ownDir = this->orientation;

	ownDir = signedOrientation(ownDir);
	faceAngle = signedOrientation(faceAngle);

	if ((ownDir - 90 - faceAngle) < (ownDir + 90 - faceAngle)) {
		faceAngle -= 90;

	}
	else {
		faceAngle -= 90;

	}

	turnToFace(faceAngle);

	return faceAngle;
}

inline int AIObject::signedOrientation(int unsignedOrientation) {
	int signedOrientation = unsignedOrientation;
	if (signedOrientation > 180) {
		signedOrientation -= 360;
	}
	return signedOrientation;
}


//returns the distance to the target
int AIObject::distanceToTarget(Vector2 origin, Vector2 destination) {
	Vector2 targetDir;
	targetDir.x = destination.x - origin.x;
	targetDir.y = destination.y - origin.y;

	int magnitude = sqrt(pow(targetDir.x, 2) + pow(targetDir.y, 2));

	return magnitude;
}

//returns the facing to the target in degrees
int AIObject::angleToTarget(Vector2 origin, Vector2 destination) {

	Vector2 targetDir;
	targetDir.x = destination.x - origin.x;
	targetDir.y = destination.y - origin.y;

	int outputAngle = atan2(targetDir.x, targetDir.y) * 180 / 3.1415;
	//if (outputAngle < 0) {
	//	outputAngle += 360;
	//}
	return outputAngle;
}

//turn ship to face specified direction
void AIObject::turnToFace(int newDir) {
	int oldDir = this->orientation;
	if (oldDir > 180)
		oldDir -= 360;

	int turnMagnitude = oldDir - newDir;

	string msgData = id + ",0,Boat_S2.png";

	if (turnMagnitude > 10) {
		msgData = id + ",0,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
	}
	else if (turnMagnitude < -10) {
		msgData = id + ",4,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
	}
	else if (turnMagnitude > -5 && turnMagnitude < 5) {
		msgData = id + ",2,Boat_S2.png";
		aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
	}
}