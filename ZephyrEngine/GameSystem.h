#pragma once
#include "System.h"
//#include "GameObject.h"
#include "FullscreenObj.h"
#include "Cannonball.h"
#include "ShipObj.h"
#include "WindArrowObj.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <thread>
#include "ObjectData.h"

extern volatile bool malive;
int volatile levelLoaded = -1;

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
	
	ObjectData objData;
	void removeAllGameObjects();

	const int timeFrame = 20;

	// -1	= no level loaded
	// 0	= Main Menu
	// 1	= Settings
	// 2	= In Game
	// 3	= Level select (if required)

private:
	void mainMenuHandler(Msg * msg);
	void lvl1Handler(Msg * msg);
	

	// The position of the marker, goes from 0 to 2, 0 being the top
	int markerPosition = 0;
};