#include "AIObject.h"

using namespace std;

AIObject::AIObject()
{
	target = NULL;
}

AIObject::~AIObject()
{
}

void AIObject::update(){

	//set the target
	if (target == NULL) {
		for (WorldObject* w : aiData->worldObjects) {
			if (w->id == "playerShip") {
				target = w;
			}
		}
	}

	colAvoidanceBehaviour();

	if (target != NULL) {
		int range = distanceToTarget(this->pos, target->pos);

		if (range > 90) {
			seekBehaviour();
		}
		else if (range < 80) {
			engageBehaviour();
		}
	}

	//set mast
	string msgData = id + ",2,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

	msgData = id + ",1,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));

	//msgData = id + ",1,Boat_S2.png";
	//aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
}

int AIObject::colAvoidanceBehaviour() {
	vector2 cDir[4];
	float x = pos.x;

	for (WorldObject* w : aiData->worldObjects) {	

		//if (w->id != "island1")
		if (w->id == id ) //don't avoid collisions with yourself
			continue;

		for (int i = 0; i < 4; i++) {
			cDir[i].x = w->c[i].x - this->pos.x;
			cDir[i].y = w->c[i].y - this->pos.y;
			//OutputDebugString(to_string(distanceToTarget(this->pos, w->c[i])).c_str());

			//TODO fix angle to look at own facing
			if (abs(distanceToTarget(pos, w->c[i])) < 70) {
				abs(angleToTarget(pos, cDir[i]) - orientation) < 20;
				OutputDebugString(id.c_str());
				OutputDebugString(" AVOIDING COLLISION WITH ");
				OutputDebugString(w->id.c_str());
				OutputDebugString("\n");
			}
		}

	}
	return 0;
}
inline int AIObject::angleBetween(vector2 v1, vector2 v2) {
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
int AIObject::distanceToTarget(vector2 origin, vector2 destination) {
	vector2 targetDir;
	targetDir.x = destination.x - origin.x;
	targetDir.y = destination.y - origin.y;

	int magnitude = sqrt(pow(targetDir.x, 2) + pow(targetDir.y, 2));
	//OutputDebugString(to_string(magnitude).c_str());
	//OutputDebugString(id.c_str());
	//OutputDebugString("\n");
	return magnitude;
}

//returns the facing to the target in degrees
int AIObject::angleToTarget(vector2 origin, vector2 destination) {
	
	vector2 targetDir;
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