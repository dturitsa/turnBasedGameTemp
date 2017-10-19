#include "Cannonball.h"

Cannonball::Cannonball(string _id, std::string renderableName, int xpos, int ypos, int _orientation, int _width, int _length, ObjectData* _objData) {
	objData = _objData;
	id = _id;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = _orientation;
	width = _width;
	length = _length;
	physicsEnabled = 1;
}

Cannonball::Cannonball(map <string, string> paramsMap, ObjectData* objData) : GameObject(paramsMap, objData) {
	
}

string Cannonball::toString() {
	string output = GameObject::toString();
	output += "\nxpos: " + to_string(x) + ",";
	output += "\nypos: " + to_string(y) + ",";
	return output;
}

Cannonball::~Cannonball() {
}

string Cannonball::getObjectType() {
	return "Cannonball";
}


void Cannonball::earlyUpdate() {

}
void Cannonball::midUpdate() {
	//OutputDebugString("Cannon miduptdate");
	//OutputDebugString("\n");
	counter++;
	

	if (counter > 90 && counter < 110) {
		std::ostringstream oss;
		oss << id << "," << "1" << "," << deathSprite;
		//Msg* m = new Msg(UPDATE_OBJ_SPRITE, oss.str());
		objData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, oss.str()));
		//objData->toPostVector.push_back(new Msg(CHANGE_MAST, "shipwreck,0,Boat_S2.png"));
		
	}
	

	if (counter > 110) {
		objData->toDestroyVector.push_back(this);
	}

}
void Cannonball::lateUpdate() {

}

void Cannonball::onCollide(GameObject* otherObj) {
	if (counter > 15) {
		objData->toDestroyVector.push_back(this);
		//deathSprite = "explosion.png";
		//counter = 107;
	}

	if (otherObj != parentObject) {
	}
	OutputDebugString(id.c_str());
	OutputDebugString(" COLLIDED WITH ");
	OutputDebugString(otherObj->id.c_str());
	OutputDebugString("\n");
	
}