#pragma once
#include "GameObject.h"

class DummyGameObj :
	public GameObject {
public:
	DummyGameObj();
	~DummyGameObj();

	std::string someTestFunction(Msg* m);
	void DummyGameObj::earlyUpdate();
	void DummyGameObj::midUpdate();
	void DummyGameObj::lateUpdate();
};

