#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

/*
//hardcoded version of loadScene(fileName) used for testing.
void GameSystem::startTestLevel() {
	
	DummyGameObj* d = new DummyGameObj(1,"DO",0,0,0);
	gameObjects.push_back(d);
	
	std::ostringstream oss;
	oss << d->id << ',' << d->renderable << ',' << d->x << ',' << d->y << ',' << d->orientation;
	//msgBus->postMessage(new Msg(NEW_OBJECT_TO_RENDER, oss.str()));

	oss.str(std::string());
}
*/
//reads gameobjects from a file. instantiates them and adds them to the list of active objects
void GameSystem::addGameObjects(string fileName) {

	std::string data = openFileRemoveSpaces(fileName);

	vector<string> splitDataVector = split(data, ';');//split gameobjects by

	GameObject* g; //new gameobject to be created
	//loop through objects read in from file
	for (int j = 0; j < splitDataVector.size(); j++) {
		
		vector<string> splitObjData = split(splitDataVector[j], ',');

		std::map<std::string, std::string> gameObjDataMap;
		//loop through elements of each GameObject and add them to the object parameter map
		for (int i = 0; i < splitObjData.size(); i++) {
			vector<string> keyValue = split(splitObjData[i], ':');
			gameObjDataMap[keyValue[0]] = keyValue[1];
		}

		//gets the gameObject type
		string gameObjectType = gameObjDataMap.find("gameObjectType")->second;
		g = NULL;
		//just hard coded else ifs for now... should probably make retreive available classes automatically
		if (gameObjectType.compare("DummyGameObj") == 0)
			g = new DummyGameObj(gameObjDataMap);

		else if (gameObjectType.compare("ShipObj") == 0) 
			g = new ShipObj(gameObjDataMap);

		else if (gameObjectType.compare("GameObject") == 0)
			g = new GameObject(gameObjDataMap);
			
		if (g != NULL)
			createGameObject(g);
	}
}

void GameSystem::saveToFIle(string fileName) {
	string output = "";
	for (GameObject* obj : gameObjects) {
		output+= obj->toString();
		output.pop_back();//remove the tailing ','
		output += ";\n";
	}	
	writeToFile(fileName, output);
}

// This function adds a created game object to the main list, and posts a message to the render
// and physics systems so that they can add it to their list as well
void GameSystem::createGameObject(GameObject* g) {
	gameObjects.push_back(g);
	std::ostringstream oss;
	oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->orientation; 
	// maybe add the rest of the variables into the oss as well, but can decide later depending on
	// what physics needs

	msgBus->postMessage(new Msg(GO_ADDED, oss.str()));
}

void GameSystem::startSystemLoop() {	
	// addGameObjects("testScene.txt");
	// saveToFIle("testScene2.txt");

	//clocks for limiting gameloop speed
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;

	// Main Game Loop
	while (true) {
		// Gameloop speed
		thisTime = clock();
		if ((thisTime - lastTime) < timeFrame) {
			continue; //if loop duration isn't passed , check again
		}

		lastTime = thisTime;

		/////////////////////////////////////////////////////////////////////
		//							OK to Run							   //
		/////////////////////////////////////////////////////////////////////

		switch (levelLoaded) {
		case -1: // First launch
			// this means we've just started up the system. We should load the main menu
			levelLoaded = 0;
			// send load menu message
			
			break;
		case 0: // Menu page

			break;
		case 1: // Setings page
			break;
		case 2: // Game loaded
			for (GameObject* obj : gameObjects) {
				obj->earlyUpdate();
			}

			// wait for all threads to complete (not yet implemented)

			for (GameObject* obj : gameObjects) {
				obj->midUpdate();
			}

			// wait for all threads to complete (not yet implemented)

			for (GameObject* obj : gameObjects) {
				obj->lateUpdate();
			}
			break;
		default:
			break;
		}
	}
}

void GameSystem::gameObjectRemoved(GameObject* g) {
	Msg* m = new Msg(GO_REMOVED, g->id);
	msgBus->postMessage(m);
}

void GameSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);
	std::ostringstream oss;
	Msg* mm = new Msg(EMPTY_MESSAGE, "");
	GameObject* g;

	if ((levelLoaded == 0) || (levelLoaded == 1)) {
		// menu & settings switch case
		switch (msg->type) {
		case UP_ARROW_PRESSED:
			// move the marker location and let rendering know
			markerPosition++;
			markerPosition = markerPosition % 3;
			break;
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know
			markerPosition--;
			markerPosition = markerPosition % 3;
			break;
		case SPACEBAR_PRESSED:
			// post message of current marker location activation?
			break;
		default:
			break;
		}
	} else if (levelLoaded == 2) {
		// game running switch case
		switch (msg->type) {
		case TEST_KEY_PRESSED:
			g = gameObjects.front();
			g->x++;
			oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->orientation;
			mm = new Msg(UPDATE_OBJECT_POSITION, oss.str());
			msgBus->postMessage(mm);
			break;
		case UP_ARROW_PRESSED:
			break;
		default:
			break;
		}
	} else {
		// -1 case; ignore since we haven't even loaded anything yet
	}
}