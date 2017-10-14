#include "Cannonball.h"

Cannonball::Cannonball(int idNum, std::string renderableName, int xpos, int ypos, int orientation) {
	id = idNum;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = orientation;
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
	return "ShipObj";
}


void Cannonball::earlyUpdate() {

}
void Cannonball::midUpdate() {

}
void Cannonball::lateUpdate() {

}