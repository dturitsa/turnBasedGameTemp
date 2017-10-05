#include "ShipObj.h"

ShipObj::ShipObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation) {
	id = idNum;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = orientation;
}

ShipObj::ShipObj(map <string, string> paramsMap) : GameObject(paramsMap) {
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