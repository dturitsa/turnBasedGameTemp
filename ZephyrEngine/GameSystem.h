#pragma once
#include "System.h"
#include "GameObject.h"
#include "DummyGameObj.h"
#include<iostream>
#include<fstream>
#include <chrono>
#include <map>
#include <algorithm>
#include <typeinfo>

class GameSystem : public System {
public:
	GameSystem(MessageBus* mbus);
	~GameSystem();

	void handleMessage(Msg * msg);

	void startSystemLoop();
	void startTestLevel();
	void addGameObjects(string fileName);
	void saveToFIle(string fileName);
	void createGameObject(GameObject* g);
	void GameSystem::gameObjectRemoved(GameObject* g);
	string getObjectType();
	std::vector<GameObject*> gameObjects;
	
	int timeFrame = 10;


private:
};