#pragma once
#include "GameObject.h"

class WindArrowObj :
	public GameObject {
public:
	WindArrowObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation);

	WindArrowObj(map<string, string> params, ObjectData* objData);
	~WindArrowObj();
	virtual std::string getObjectType();
	void earlyUpdate();
	void midUpdate();
	void lateUpdate();
};
