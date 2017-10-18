#pragma once
#include "GameObject.h"

class GameObjectManager
{
public:
	std::vector<GameObject*> gameObjects;
	GameObjectManager();
	~GameObjectManager();
	void GameObjectManager::gameObjectRemoved(GameObject* g);
};

