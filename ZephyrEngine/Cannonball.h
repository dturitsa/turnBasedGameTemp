#pragma once
#include "GameObject.h"


class Cannonball :
	public GameObject {
public:
	Cannonball(string _id, std::string renderableName, int xpos, int ypos, int _orientation, int _width, int _height);
	Cannonball(map<string, string> params, ObjectData* objData);
	~Cannonball();

	virtual std::string getObjectType();
	virtual std::string toString();
	void Cannonball::earlyUpdate();
	void Cannonball::midUpdate() override;
	void Cannonball::lateUpdate();
	void onCollide(std::string otherObjId) override;

	int counter = 0;
};

