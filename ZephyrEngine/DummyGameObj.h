#pragma once
#include "GameObject.h"


class DummyGameObj :
	public GameObject {
public:
	DummyGameObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation);
	//DummyGameObj(std::vector<std::string> params);
	DummyGameObj(map<string, string> params);
	~DummyGameObj();

	std::string someTestFunction(Msg* m);
	void DummyGameObj::earlyUpdate();
	void DummyGameObj::midUpdate();
	void DummyGameObj::lateUpdate();

	int counter = 0;
};

