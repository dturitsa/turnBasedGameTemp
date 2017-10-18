#include "ShipObj.h"

ShipObj::ShipObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation) {
	id = idNum;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = orientation;
}

ShipObj::ShipObj(map <string, string> paramsMap, ObjectData* objData) : GameObject(paramsMap, objData) {
	try {	
		sail = stoi(paramsMap.find("sail")->second);
		rudder = stoi(paramsMap.find("rudder")->second);
	}
	catch (const exception& e) {
		cout << "\n" << e.what(); // information from length_error printed
	}
}

string ShipObj::toString() {
	string output = GameObject::toString();
	output += "\nsail: " + to_string(sail) + ",";
	output += "\nrudder: " + to_string(rudder) + ",";
	return output;
}

ShipObj::~ShipObj() {
}

string ShipObj::getObjectType() {
	return "ShipObj";
}


void ShipObj::earlyUpdate() {

}
void ShipObj::midUpdate() {

}
void ShipObj::lateUpdate() {

}

void ShipObj::onCollide(std::string otherObjId) {
	if(id != "playerShip")
		health -= 10;

	if (health < 0) {
		objData->toDestroyVector.push_back(this);
	}
	OutputDebugString(id.c_str());
	OutputDebugString(" COLLIDED WITH ");
	OutputDebugString(otherObjId.c_str());
	OutputDebugString("\n");


}