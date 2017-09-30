#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

void GameSystem::startTestLevel() {
	DummyGameObj* d = new DummyGameObj(1,"DO",0,0,0);
	gameObjects.push_back(d);
	std::ostringstream oss;
	oss << d->id << ',' << d->renderable << ',' << d->x << ',' << d->y << ',' << d->orientation;
	msgBus->postMessage(new Msg(NEW_OBJECT_TO_RENDER, oss.str()));
}

void GameSystem::startSystemLoop() {
	startTestLevel();

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