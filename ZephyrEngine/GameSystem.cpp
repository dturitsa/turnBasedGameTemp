#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

void GameSystem::startTestLevel() {
	DummyGameObj* d = new DummyGameObj();
	gameObjects.push_back(d);
}

void GameSystem::startSystemLoop() {
	// Main Game Loop
	while (true) {
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

	// personal call 
	switch (msg->type) {
	case OBJ_TEST_MSG:
	break;
	default:
		break;
	}
}