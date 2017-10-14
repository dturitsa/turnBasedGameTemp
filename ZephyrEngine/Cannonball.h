#pragma once
#include "GameObject.h"


class Cannonball :
	public GameObject {
public:
	Cannonball(int idNum, std::string renderableName, int xpos, int ypos, int orientation);
	Cannonball(map<string, string> params);
	~Cannonball();

	virtual std::string getObjectType();
	virtual std::string toString();
	void Cannonball::earlyUpdate();
	void Cannonball::midUpdate();
	void Cannonball::lateUpdate();

	int counter = 0;
};

