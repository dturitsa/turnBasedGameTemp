#include "Cannonball.h"

Cannonball::Cannonball(string _id, std::string renderableName, int xpos, int ypos, int _orientation, int _width, int _length) {
	id = _id;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = _orientation;
	width = _width;
	length = _length;
	physicsEnabled = 1;
}

Cannonball::Cannonball(map <string, string> paramsMap) : GameObject(paramsMap) {
	
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

}
void Cannonball::lateUpdate() {

}