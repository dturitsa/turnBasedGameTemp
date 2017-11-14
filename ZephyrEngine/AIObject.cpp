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


	
	//OutputDebugString(to_string(target->x).c_str());
	//OutputDebugString(id.c_str());
	//OutputDebugString("\n");


	string msgData = id + ",2,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

	msgData = id + ",1,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));

	msgData = id + ",1,Boat_S2.png";
	aiData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));
}
