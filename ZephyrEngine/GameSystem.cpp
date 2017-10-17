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
		if (gameObjectType.compare("ShipObj") == 0) {
			g = new ShipObj(gameObjDataMap);
		}
		else if (gameObjectType.compare("GameObject") == 0) {
			g = new GameObject(gameObjDataMap);
		}
		else if (gameObjectType.compare("FullscreenObj") == 0) {
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
		output += obj->toString();
		output.pop_back();//remove the tailing ','
		output += ";\n";
	}
	writeToFile(fileName, output);
}

// This function adds a created game object to the main list, and posts a message to the render
// and physics systems so that they can add it to their list as well
void GameSystem::createGameObject(GameObject* g) {
	for (GameObject* obj : gameObjects) {
		if (g->id == obj->id) {
			//g->id.append(to_string(rand()));
			return;
		}
	}
	gameObjects.push_back(g);
	std::ostringstream oss;
	oss << g->id << ','
		<< g->renderable << ','
		<< g->x << ',' << g->y << ',' << g->z << ','
		<< g->orientation << ','
		<< g->width << ',' << g->length << ','
		<< g->physicsEnabled << ','
		//<< g->getObjectType();
		<< g->renderable;
	// maybe add the rest of the variables into the oss as well, but can decide later depending on
	// what physics needs

	msgBus->postMessage(new Msg(GO_ADDED, oss.str()), this);
}



void GameSystem::startSystemLoop() {
	//clocks for limiting gameloop speed
	clock_t thisTime = clock();

	while (true) {
		thisTime = clock();
		if (thisTime < timeFrame) {
			Sleep(timeFrame - thisTime);
		}
		timeFrame += 20;

		handleMsgQ();

		std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
		OutputDebugString("GameSystem Loop on thread: ");
		OutputDebugString(s.c_str());
		OutputDebugString("\n");

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
		gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
	}

	//gameObjects.clear();
}

void GameSystem::gameObjectRemoved(GameObject* g) {
	Msg* m = new Msg(GO_REMOVED, g->id);
	msgBus->postMessage(m, this);
}

void GameSystem::handleMessage(Msg *msg) {

	// call the parent first 
	System::handleMessage(msg);

	std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
	OutputDebugString("GameSystem Handle on thread: ");
	OutputDebugString(s.c_str());
	OutputDebugString("\n");

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
			//OutputDebugString("MarkerPos: " + markerPosition + '\n');
			oss << "obj3,Z6_Marker_P" << markerPosition << ".png," << "0,0,10,0,200,200,0,0";
			mm->type = UPDATE_OBJECT_POSITION;
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case UP_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition--;
			if (markerPosition < 0) {
				markerPosition = 0;
			}
			markerPosition = markerPosition % 3;

			oss << "obj3,Z6_Marker_P" << markerPosition << ".png," << "0,0,10,0,200,200,0,0";
			mm->type = UPDATE_OBJECT_POSITION;
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case SPACEBAR_PRESSED:
			if (markerPosition == 2) {
				// Exit was selected
				mm->type = EXIT_GAME;
				msgBus->postMessage(mm, this);
			}

			if (markerPosition == 1) {
				// Go to settings
			}
			else if (markerPosition == 0) {
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

	}
	else if (levelLoaded == 1) {
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
	}
	else if (levelLoaded == 2) {
		// game running switch case
		switch (msg->type) {
		case TEST_KEY_PRESSED:
			for (GameObject* g : gameObjects) {
				if (g->id == "shipwreck") {
					gameObjectRemoved(g);
					gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), g), gameObjects.end());
					/*
					g->x++;
					g->y++;
					g->orientation += 10.0;
					oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->z << ',' << g->orientation << ",20,20,1,0";
					mm = new Msg(UPDATE_OBJECT_POSITION, oss.str());
					msgBus->postMessage(mm);
					*/

				}
			}
			break;
		case GO_COLLISION: {
			vector<string> data = split(msg->data, ',');

			for (GameObject* g : gameObjects) {
				//OutputDebugString(g->id.c_str());

				if (g->id == data[0] && data[0] != "playerShip" && data[1] != "playerShip") {
					OutputDebugString(data[0].c_str());
					OutputDebugString(" Collided With ");
					OutputDebugString(data[1].c_str());
					OutputDebugString("\n");
					gameObjectRemoved(g);
				}

			}
			break;
		}
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
					msgBus->postMessage(mm, this);

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
					msgBus->postMessage(mm, this);
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
					msgBus->postMessage(mm, this);
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
					msgBus->postMessage(mm, this);
					break;
				}
			}
			break;
		case SPACEBAR_PRESSED: {
			// fire a cannon ball. update later for more functionality
			// use random id for now
			srand(time(NULL));
			int randomNum = std::rand();

			int cx = 0;
			int cy = 0;
			int corient = 0;

			// find the ship so that we know where to create the cball
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					cx = g->x;
					cy = g->y;
					corient = g->orientation + 90;
					break;
				}
			}

			Cannonball* c = new Cannonball(to_string(randomNum), "tempCannonball.png", cx, cy, corient, 5, 5);

			// post cannon ball obj to systems
			createGameObject(c);

			break;
		}
		case UPDATE_OBJECT_POSITION: {

			vector<string> data = split(msg->data, ',');

			for (GameObject* g : gameObjects) {
				//OutputDebugString(g->id.c_str());

				if (g->id == data[0]) {
					//OutputDebugString(data[0].c_str());
					//OutputDebugString("\n");

					g->x = atof(data[2].c_str());
					g->y = atof(data[3].c_str());
					g->orientation = atof(data[5].c_str());
				}

			}
			break;
		}
		default:
			break;
		}
	}
	else {
		// -1 case; ignore since we haven't even loaded anything yet
	}
}