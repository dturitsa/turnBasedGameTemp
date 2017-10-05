#include "DummyGameObj.h"

DummyGameObj::DummyGameObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation) {
	id = idNum;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	GameObject::orientation = orientation;
}

DummyGameObj::DummyGameObj(map <string, string> paramsMap) : GameObject(paramsMap) {
	try {
		dummyLevel = stoi(paramsMap.find("dummyLevel")->second);
	}
	catch (const exception& e) { 
		cout << "\n" << e.what(); // information from length_error printed
	}
}

string DummyGameObj::toString() {
	string output = GameObject::toString();
	output += "\ndummyLevel: " + to_string(dummyLevel) + ",";
	return output;
}

DummyGameObj::~DummyGameObj() {
}

string DummyGameObj::getObjectType() {
	return "DummyGameObj";
}

std::string DummyGameObj::someTestFunction(Msg* m) {
	std::string temp = m->data;
	std::vector<std::string> v = split(temp, ',');
	return v.front();
}

void DummyGameObj::earlyUpdate() {
	Msg* m = new Msg(OBJ_TEST_MSG, "MUAHAHAHAHA, HAHA, HA");
	std::string s = someTestFunction(m);
}
void DummyGameObj::midUpdate() {
	Msg* m = new Msg(OBJ_TEST_MSG, "HAHAHA, HAHA, HA");
	std::string s = someTestFunction(m);
}
void DummyGameObj::lateUpdate() {
	Msg* m = new Msg(OBJ_TEST_MSG, "BAHAHAHA, HAHA, HA");
	std::string s = someTestFunction(m);
}