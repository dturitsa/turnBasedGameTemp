#include "GameObject.h"
using namespace std;
GameObject::GameObject() {
}

GameObject::~GameObject() {
}
GameObject::GameObject(map <string, string> paramsMap) {

	try {
		id = paramsMap.find("id")->second;
		renderable = paramsMap.find("renderable")->second;
		x = stoi(paramsMap.find("xPos")->second);
		y = stoi(paramsMap.find("yPos")->second);
		orientation = stoi(paramsMap.find("orientation")->second);
	}
	catch (const exception& e) {
		cout << e.what(); // information from length_error printed
	}
}
string GameObject::toString() {
	string output = "\ngameObjectType: " + getObjectType() + ",";
	output += "\nid: " + id + ",";
	output += "\nrenderable: " + renderable + ",";
	output += "\nxPos: " + to_string(x)+ ",";
	output += "\nyPos: " + to_string(y) + ",";
	output += "\norientation: " + to_string(orientation) + ",";
	return output;
}

string GameObject::getObjectType() {
	return "GameObject";
}

void GameObject::earlyUpdate() {

}

void GameObject::midUpdate() {

}

void GameObject::lateUpdate() {

}