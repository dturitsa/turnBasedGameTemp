#pragma once
#include "GameObject.h"


class ShipObj :
	public GameObject {
public:
	ShipObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation);
	//DummyGameObj(std::vector<std::string> params);
	ShipObj(map<string, string> params);
	~ShipObj();
	virtual std::string getObjectType();
	virtual std::string toString();
	void ShipObj::earlyUpdate();
	void ShipObj::midUpdate();
	void ShipObj::lateUpdate();

	int sail;//the sail position 0-2
	int rudder;// 0-4 rudder position

	int counter = 0;
};

