#pragma once
//#include "GameObject.h"
#include <vector>
#include "Msg.h"

class AIObject;
class WorldObject;
class AIData
{
public:

	std::vector<Msg*> toPostVector;
	std::vector<WorldObject*> worldObjects;
	//std::vector<AIObject*> toCreateVector;
	//std::vector<AIObject*> toDestroyVector;

	//ObjectData();
	//	~ObjectData();
};
