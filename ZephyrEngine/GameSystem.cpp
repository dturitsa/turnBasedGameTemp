#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

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
		//just hard coded else ifs for now... should probably make retreive available classes automatically <- Did some research, cpp doesn't support reflection (Hank)
		if (gameObjectType.compare("DummyGameObj") == 0) {
			g = new DummyGameObj(gameObjDataMap);
		} else if (gameObjectType.compare("ShipObj") == 0) {
			g = new ShipObj(gameObjDataMap);
		} else if (gameObjectType.compare("GameObject") == 0) {
			g = new GameObject(gameObjDataMap);
		} else if (gameObjectType.compare("FullscreenObj") == 0) {
			g = new FullscreenObj(gameObjDataMap);
		}

		if (g != NULL) {
			createGameObject(g);
		}
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
	oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->z << ',' << g->direction << ',' << g->width << ',' << g->length << ',' << g->orientation;
	// maybe add the rest of the variables into the oss as well, but can decide later depending on
	// what physics needs

	msgBus->postMessage(new Msg(GO_ADDED, oss.str()));
}


// saveToFIle("testScene2.txt");

void GameSystem::startSystemLoop() {	
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
			// Load Main Menu Scene
			addGameObjects("main_menu.txt"); 
			break;
		case 0: // Menu page
			// does nothing as user changes are handled inside handleMessage. In this state,
			// the only thing we could possibly do is... idk yet.
			break;
		case 1: // Settings page
			// does nothing as user changes are handled inside handleMessage. In this state,
			// the only thing we could possibly do is... idk yet. Basically same shit as
			// the Menu page tho
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

// wrapper that removes all game objects by sending a message for each object.
// the other possible way to do this is by using a functiona nd then posting a specific
// message to the render system, but i don't wanna touch the render system right now
void GameSystem::removeAllGameObjects() {
	for (GameObject* go : gameObjects) {
		gameObjectRemoved(go);
	}

	gameObjects.clear();
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

	if (levelLoaded == 0) {
		// main menu switch case
		switch (msg->type) {
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition++;
			markerPosition = markerPosition % 3;
			oss << "obj3,Z6_Marker_P" << markerPosition << ".png," << "0,0,0,0,0,1920,1080,0";
			mm->type = UPDATE_OBJECT_POSITION;
			mm->data = oss.str();
			msgBus->postMessage(mm);
			break;
		case UP_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition--;
			if (markerPosition < 0) {
				markerPosition = 0;
			}
			markerPosition = markerPosition % 3;
			oss << "obj3,Z6_Marker_P" << markerPosition << ".png," << "0,0,0,0,0,1920,1080,0";
			mm->type = UPDATE_OBJECT_POSITION;
			mm->data = oss.str();
			msgBus->postMessage(mm);
			break;
		case SPACEBAR_PRESSED:
			if (markerPosition == 2) {
				// Exit was selected
				mm->type = EXIT_GAME;
				msgBus->postMessage(mm);
			}

			if (markerPosition == 1) {
				// Go to settings
			} else if (markerPosition == 0) {
				// start the game (or go to level select?)
				// first, clear all objects
				removeAllGameObjects();

				// then, load new objects
				addGameObjects("Alpha_Level_1.txt"); // TEMPORARY 
				levelLoaded = 2;
			}
			break;
		default:
			break;
		}
		// OutputDebugString("Ls:" + markerPosition + '\n'); // tilt < can't use this
	} else if (levelLoaded == 1) {
		// settings menu
		switch (msg->type) {
		case UP_ARROW_PRESSED:
			// move the marker location and let rendering know
			markerPosition++;
			markerPosition = markerPosition % 3;
			break;
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know
			markerPosition--;
			if (markerPosition < 0) {
				markerPosition = 0;
			}
			markerPosition = markerPosition % 3;
			break;
		case SPACEBAR_PRESSED:
			// post message of current marker location activation?
			break;
		default:
			break;
		}
	}  else if (levelLoaded == 2) {
		// game running switch case
		switch (msg->type) {
		case TEST_KEY_PRESSED:
			g = gameObjects.front();
			g->x++;
			oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->z << ',' << g->orientation;
			mm = new Msg(UPDATE_OBJECT_POSITION, oss.str());
			msgBus->postMessage(mm);
			break;
		case UP_ARROW_PRESSED:
			// increase mast
			// find the ship obj, and when you find it, increase mast
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->sail++;
					if (so->sail > 2) {
						so->sail = 2;
					}
					mm->type = CHANGE_MAST;
					oss << so->id << "," << so->sail;
					mm->data = oss.str();
					msgBus->postMessage(mm);
					break;
				}
			}
			break;
		case DOWN_ARROW_PRESSED:
			// decrease mast
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->sail--;
					if (so->sail < 0) {
						so->sail = 0;
					}

					mm->type = CHANGE_MAST;
					oss << so->id << "," << so->sail;
					mm->data = oss.str();
					msgBus->postMessage(mm);
					break;
				}
			}
			break;
		case RIGHT_ARROW_PRESSED:
			// change rudder to right
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->rudder++;
					if (so->sail > 4) {
						so->sail = 4;
					}

					mm->type = CHANGE_RUDDER;
					oss << so->id << "," << so->rudder;
					mm->data = oss.str();
					msgBus->postMessage(mm);
					break;
				}
			}
			break;
		case LEFT_ARROW_PRESSED:
			// change rudder left
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->rudder--;
					if (so->rudder < 0) {
						so->rudder = 0;
					}

					mm->type = CHANGE_RUDDER;
					oss << so->id << "," << so->rudder;
					mm->data = oss.str();
					msgBus->postMessage(mm);
					break;
				}
			}
			break;
		case SPACEBAR_PRESSED:
			// fire a cannon ball. update later for more functionality
			// Cannonball c = new Cannonball();
			// create cannon ball obj
			// post cannon ball obj to systems
			break;
		default:
			break;
		}
	} else {
		// -1 case; ignore since we haven't even loaded anything yet
	}
}