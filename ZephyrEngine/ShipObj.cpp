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

	if (id == "enemy1") {
		string msgData = id + ",2,Boat_S2.png";
		objData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

		msgData = id + ",1,Boat_S2.png";
		objData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));

		msgData = id + ",1,Boat_S2.png";
		objData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));
	}

	if (id == "enemy2") {
		string msgData = id + ",1,Boat_S2.png";
		objData->toPostVector.push_back(new Msg(CHANGE_MAST, msgData));

		msgData = id + ",4,Boat_S2.png";
		objData->toPostVector.push_back(new Msg(CHANGE_RUDDER, msgData));

		msgData = id + ",1,Boat_S1.png";
		objData->toPostVector.push_back(new Msg(UPDATE_OBJ_SPRITE, msgData));
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
	counter++;
	if (counter == 150 && id != "playerShip") {
		shoot("left");
	}
	else if (counter == 200 && id != "playerShip") {
		shoot("right");
		counter = 0;
	}
}
void ShipObj::lateUpdate() {

}

void ShipObj::onCollide(GameObject* otherObj) {

	if (otherObj->parentObject == this) {
		return;
	}
		

	if (id != "playerShip") {
		health -= 7;
	}
		

	if (health < 0) {
		objData->toDestroyVector.push_back(this);
	}
	OutputDebugString(id.c_str());
	OutputDebugString(" COLLIDED WITH ");
	OutputDebugString(otherObj->id.c_str());
	OutputDebugString("\n");
}

//direction = "left",  "right", forward
void ShipObj::shoot(string direction) {
	//srand(time(NULL));
	int shootDir;
	int randomNum = std::rand();
	if (direction == "right") {
		 shootDir = orientation + 90;
	}
	else if (direction == "left") {
		shootDir = orientation + -90;
	} 
	else if (direction == "forward") {
		shootDir = orientation;
	}
	
	Cannonball* c = new Cannonball(to_string(randomNum), "cannon_ball.png", x, y, shootDir, 5, 5, objData);
	c->parentObject = this;
	objData->toCreateVector.push_back(c);


}