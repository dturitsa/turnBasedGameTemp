#pragma once
#include "System.h"
#include "GameObject.h"
#include "DummyGameObj.h"
#include "ShipObj.h"
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
	//void startTestLevel();
	void addGameObjects(string fileName);
	void saveToFIle(string fileName);
	void createGameObject(GameObject* g);
	void GameSystem::gameObjectRemoved(GameObject* g);
	std::vector<GameObject*> gameObjects;
	
	int timeFrame = 10;

	// -1	= no level loaded
	// 0	= Main Menu
	// 1	= Settings
	// 2	= In Game
	int levelLoaded = -1;

private:

	// The position of the marker, goes from 0 to 2, 0 being the top
	int markerPosition = 0;
};