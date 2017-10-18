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
	
	if (counter > 100) {
		objData->toDestroyVector.push_back(this);
	}

}
void Cannonball::lateUpdate() {

}

void Cannonball::onCollide(std::string otherObjId) {
	if (counter > 12) {
		objData->toDestroyVector.push_back(this);
	}

	
}