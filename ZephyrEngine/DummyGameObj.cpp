#include "DummyGameObj.h"

DummyGameObj::DummyGameObj() {
}

DummyGameObj::~DummyGameObj() {
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