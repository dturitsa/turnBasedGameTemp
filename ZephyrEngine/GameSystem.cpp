#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

//hardcoded version of loadScene(fileName) used for testing.
void GameSystem::startTestLevel() {
	
	DummyGameObj* d = new DummyGameObj(1,"DO",0,0,0);
	gameObjects.push_back(d);
	
	std::ostringstream oss;
	oss << d->id << ',' << d->renderable << ',' << d->x << ',' << d->y << ',' << d->orientation;
	//msgBus->postMessage(new Msg(NEW_OBJECT_TO_RENDER, oss.str()));

	oss.str(std::string());
}

//reads gameobjects from a file. instantiates them and adds them to the list of active objects
void GameSystem::addGameObjects(string fileName) {

	//open file and read it into a string
	std::ifstream t(fileName);
	std::string output((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	t.close();

	output.erase(std::remove_if(output.begin(), output.end(), isspace), output.end());//remove spaces from data

	vector<string> splitDataVector = split(output, ';');//split gameobjects by

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
		if(gameObjectType.compare("DummyGameObj"))
			g = new DummyGameObj(gameObjDataMap);

		else if (gameObjectType.compare("GameObj"))
			g = new DummyGameObj(gameObjDataMap);
			
		if (g != NULL)
			createGameObject(g);
	}
}

//
void GameSystem::createGameObject(GameObject* g) {
	gameObjects.push_back(g);
	std::ostringstream oss;
	oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->orientation;
	msgBus->postMessage(new Msg(NEW_OBJECT_TO_RENDER, oss.str()));
	
}

void GameSystem::startSystemLoop() {
	addGameObjects("testScene.txt");
	addGameObjects("testScene2.txt");
	//startTestLevel();

	//clocks for limiting gameloop speed
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;

	// Main Game Loop
	while (true) {

		thisTime = clock();
		if ((thisTime - lastTime) < 100)
			continue; //if loop duration isn't passed , check again

		lastTime = thisTime;

		for (GameObject* obj : gameObjects ) {
			obj->earlyUpdate();
		}

		// wait for all threads to complete

		for (GameObject* obj : gameObjects) {
			obj->midUpdate();
		}

		// wait for all threads to complete

		for (GameObject* obj : gameObjects) {
			obj->lateUpdate();
		}
	}
}

void GameSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);
	std::ostringstream oss;
	Msg* mm = new Msg(EMPTY_MESSAGE, "");
	GameObject* g;
	// personal call 
	switch (msg->type) {
	case TEST_KEY_PRESSED:
		g = gameObjects.front();
		g->x++;
		oss << g->id << ',' << g->renderable << ',' << g->x << ',' << g->y << ',' << g->orientation;
		mm = new Msg(UPDATE_OBJECT_POSITION, oss.str());
		msgBus->postMessage(mm);
	break;
	default:
		break;
	}
}