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

	if (target != NULL) {
		int faceAngle = angleToTarget(this->pos, target->pos);
		int range = distanceToTarget(this->pos, target->pos);

		if (range > 90) {
			turnToFace(faceAngle);
		}
		else if (range < 80) {
			int oldDir = this->orientation;
			if (oldDir > 180)
				oldDir -= 360;

			if(oldDir - faceAngle > 0)
				turnToFace(faceAngle + 90);
			else
				turnToFace(faceAngle - 90);
		}
	}
	string msgData = id + ",2,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

	msgData = id + ",1,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));

	//msgData = id + ",1,Boat_S2.png";
	//aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));
}

int AIObject::distanceToTarget(vector2 origin, vector2 destination) {
	vector2 targetDir;
	targetDir.x = destination.x - origin.x;
	targetDir.y = destination.y - origin.y;

	int magnitude = sqrt(pow(targetDir.x, 2) + pow(targetDir.y, 2));
	OutputDebugString(to_string(magnitude).c_str());
	//OutputDebugString(id.c_str());
	OutputDebugString("\n");
	return magnitude;
}

//returns the facing to the target
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