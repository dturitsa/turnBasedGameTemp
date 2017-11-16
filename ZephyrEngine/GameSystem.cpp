#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {
	objData = *(new ObjectData());
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
			g = new ShipObj(gameObjDataMap, &objData);
		}
		else if (gameObjectType.compare("GameObject") == 0) {
			g = new GameObject(gameObjDataMap, &objData);
		}
		else if (gameObjectType.compare("FullscreenObj") == 0) {
			g = new FullscreenObj(gameObjDataMap, &objData);
		}
		else if (gameObjectType.compare("WindArrowObj") == 0) {
			g = new WindArrowObj(gameObjDataMap, &objData);
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
			OutputDebugString("error id found");
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
		<< g->getObjectType();
	//<< g->renderable;
	// maybe add the rest of the variables into the oss as well, but can decide later depending on
	// what physics needs

	msgBus->postMessage(new Msg(GO_ADDED, oss.str()), this);
}



void GameSystem::startSystemLoop() {
	//clocks for limiting gameloop speed
	clock_t thisTime = clock();

	int currentGameTime = 0;
	while (alive) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}
		currentGameTime = thisTime + timeFrame;

		handleMsgQ();

		////Display Thread ID for Debugging
		//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
		//OutputDebugString("GameSystem Loop on thread: ");
		//OutputDebugString(s.c_str());
		//OutputDebugString("\n");

		/////////////////////////////////////////////////////////////////////
		//							OK to Run							   //
		/////////////////////////////////////////////////////////////////////

		Msg* m = new Msg(EMPTY_MESSAGE, "");

		switch (levelLoaded) {
		case -1: // First launch
			// this means we've just started up the system. We should load the main menu
			levelLoaded = 0;
			m = new Msg(LEVEL_LOADED, "0");
			msgBus->postMessage(m, this);

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
		case 2: { // Game loaded
			bool endgame = false;
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

			//loop through list of objects to create added by the gameobjects
			for each (GameObject* c in objData.toCreateVector) {
				createGameObject(c);
			}
			objData.toCreateVector.clear();

			//loop through list of objects to destroy added by the gameobjects
			for each (GameObject* g in objData.toDestroyVector) {
				gameObjectRemoved(g);

				// increase score
				if (g->id != "playerShip") {

				}

				// end game
				if (g->id == "playerShip") {
					endgame = true;
				}

				gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), g), gameObjects.end());
			}
			objData.toDestroyVector.clear();


			//loop through list of messages to send that were added by Game objects
			for each (Msg* m in objData.toPostVector) {
				msgBus->postMessage(m, this);
			}
			objData.toPostVector.clear();

			if (endgame) {
				removeAllGameObjects();

				levelLoaded = 3;
				m = new Msg(LEVEL_LOADED, "3");
				msgBus->postMessage(m, this);
				markerPosition = 1;

				// Load Main Menu Scene
				addGameObjects("gameover_menu.txt");

				// add score code here
			}
			break;
		}
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
		//gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
	}

	gameObjects.clear();
}

void GameSystem::gameObjectRemoved(GameObject* g) {
	Msg* m = new Msg(GO_REMOVED, g->id);
	msgBus->postMessage(m, this);
}

void GameSystem::handleMessage(Msg *msg) {

	// call the parent first 
	System::handleMessage(msg);

	////Display Thread ID for Debugging
	//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
	//OutputDebugString("GameSystem Handle on thread: ");
	//OutputDebugString(s.c_str());
	//OutputDebugString("\n");

	std::ostringstream oss;
	Msg* mm = new Msg(EMPTY_MESSAGE, "");
	GameObject* g;

	if (levelLoaded == 0) {
		// main menu switch case
		switch (msg->type) {
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition++;
			if (markerPosition > 2) {
				markerPosition = 2;
			}

			mm->type = UPDATE_OBJ_SPRITE;
			oss <<"obj3,1,Z6_Marker_P" << markerPosition << ".png,";
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

			mm->type = UPDATE_OBJ_SPRITE;
			oss << "obj3,1,Z6_Marker_P" << markerPosition << ".png,";
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case SPACEBAR_PRESSED:
			if (markerPosition == 2) {
				// Exit was selected, kill main
				malive = false;
			}

			if (markerPosition == 1) {
				// Go to settings
				removeAllGameObjects();
				addGameObjects("settings_menu.txt");
				levelLoaded = 1;
				markerPosition = 0;
				Msg* m = new Msg(LEVEL_LOADED, "1");
				msgBus->postMessage(m, this);
			}
			else if (markerPosition == 0) {
				// start the game (or go to level select?)
				// first, clear all objects
				removeAllGameObjects();

				// then, load new objects
				addGameObjects("Alpha_Level_1.txt"); // TEMPORARY 
				levelLoaded = 2;
				Msg* m = new Msg(LEVEL_LOADED, "2");
				msgBus->postMessage(m, this);
				score = 0;
			}
			break;
		default:
			break;
		}

	}
	else if (levelLoaded == 1) {
		// settings menu
		switch (msg->type) {
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition++;
			if (markerPosition > 2) {
				markerPosition = 2;
			}

			mm->type = UPDATE_OBJ_SPRITE;
			oss << "obj3,1,Z6_Marker_P" << markerPosition << ".png,";
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

			mm->type = UPDATE_OBJ_SPRITE;
			oss << "obj3,1,Z6_Marker_P" << markerPosition << ".png,";
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case SPACEBAR_PRESSED:
			if (markerPosition == 2) {
				// Back button, go to menu
				removeAllGameObjects();
				addGameObjects("main_menu.txt");
				levelLoaded = 0;
				markerPosition = 0;
				Msg* m = new Msg(LEVEL_LOADED, "0");
				msgBus->postMessage(m, this);
			}

			if (markerPosition == 1) {
				// change game sound to "off"
				mm->type = AUDIO_MUTE;
				mm->data = "1";
				msgBus->postMessage(mm, this);
			} else if (markerPosition == 0) {
				// change game sound to "on"
				mm->type = AUDIO_MUTE;
				mm->data = "0";
				msgBus->postMessage(mm, this);
			}
			break;
		default:
			break;
		}
	}
	else if (levelLoaded == 2) {
		//lvl1Handler(msg);
		// game running switch case
		switch (msg->type) {
		case TEST_KEY_PRESSED:
			for (GameObject* g : gameObjects) {
				if (g->id == "shipwreck") {
					Msg* m = new Msg(CHANGE_MAST,"shipwreck,2,Boat_S2.png");
					msgBus->postMessage(m, this);

					Msg* m2 = new Msg(CHANGE_RUDDER, "shipwreck,1");
					msgBus->postMessage(m2, this);
					//gameObjectRemoved(g);
					//gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), g), gameObjects.end());
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
				if (g->id == data[0]) {
					for (GameObject* o : gameObjects) {
						if (o->id == data[1]) {
							g->onCollide(o);
							break;
						}
					}
				}

			}
			break;
		}
		case KEY_W_PRESSED:
			// increase mast
			// find the ship obj, and when you find it, increase mast
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->sail++;
					if (so->sail > 2) {
						so->sail = 2;
					}


					// change sail image
					std::ostringstream tOss;
					tOss << "Boat_S" << so->sail << ".png";
					so->renderable = tOss.str();

					mm->type = CHANGE_MAST;
					oss << so->id << "," << so->sail << "," << so->renderable;
					mm->data = oss.str();
					msgBus->postMessage(mm, this);

					Msg* mm2 = new Msg(UPDATE_OBJ_SPRITE, oss.str());
					msgBus->postMessage(mm2, this);

					break;
				}
			}
			break;
		case KEY_S_PRESSED:
			// decrease mast
			for (GameObject* g : gameObjects) {
				if (g->getObjectType() == "ShipObj") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->sail--;
					if (so->sail < 0) {
						so->sail = 0;
					}

					// change sail image
					std::ostringstream tOss;
					tOss << "Boat_S" << so->sail << ".png";
					so->renderable = tOss.str();

					mm->type = CHANGE_MAST;
					oss << so->id << "," << so->sail << "," << so->renderable;
					mm->data = oss.str();
					msgBus->postMessage(mm, this);

					Msg* mm2 = new Msg(UPDATE_OBJ_SPRITE, oss.str());
					msgBus->postMessage(mm2, this);
					break;
				}
			}
			break;
		case KEY_D_PRESSED:
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
		case KEY_A_PRESSED:
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
		case KEY_E_PRESSED: {
			// fire a cannon ball to the right. 

			for (GameObject* g : gameObjects) {
				if (g->id == "playerShip") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->shoot("right");
					break;
				}
			}

			break;
		}
		case KEY_Q_PRESSED: {
			// fire a cannon ball to the left.
			for (GameObject* g : gameObjects) {
				if (g->id == "playerShip") {
					ShipObj* so = dynamic_cast<ShipObj*>(g);
					so->shoot("left");
					break;
				}
			}
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
	} else if (levelLoaded == 3) {
		switch (msg->type) {
		case DOWN_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition++;
			if (markerPosition > 2) {
				markerPosition = 2;
			}

			mm->type = UPDATE_OBJ_SPRITE;
			oss << "obj3,1,Z6_Marker_P" << markerPosition << ".png,";
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case UP_ARROW_PRESSED:
			// move the marker location and let rendering know?
			markerPosition--;
			if (markerPosition < 1) {
				markerPosition = 1;
			}
			markerPosition = markerPosition % 3;

			mm->type = UPDATE_OBJ_SPRITE;
			oss << "obj3,1,Z6_Marker_P" << markerPosition << ".png,";
			mm->data = oss.str();
			msgBus->postMessage(mm, this);
			break;
		case SPACEBAR_PRESSED:
			// End Game Screen
			if (markerPosition == 2) {
				// go to menu
				removeAllGameObjects();
				addGameObjects("main_menu.txt");
				levelLoaded = 0;
				markerPosition = 0;
				Msg* m = new Msg(LEVEL_LOADED, "0");
				msgBus->postMessage(m, this);
			}

			if (markerPosition == 1) {
				// start the game (or go to level select?)
				// first, clear all objects
				removeAllGameObjects();

				// then, load new objects
				addGameObjects("Alpha_Level_1.txt"); // TEMPORARY 
				levelLoaded = 2;
				Msg* m = new Msg(LEVEL_LOADED, "2");
				msgBus->postMessage(m, this);
				score = 0;
			}
		default:
			break;
		}
	} else {
		// -1 case; ignore since we haven't even loaded anything yet
	}
}

//should probably break the massive switch statements into functions or even classes
void GameSystem::mainMenuHandler(Msg * msg) {

}

//should probably break the massive switch statements into functions or even classes
void GameSystem::lvl1Handler(Msg * msg) {

}