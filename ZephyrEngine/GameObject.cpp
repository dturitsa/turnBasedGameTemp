#include "GameObject.h"

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
void GameObject::earlyUpdate() {

}

void GameObject::midUpdate() {

}

void GameObject::lateUpdate() {

}