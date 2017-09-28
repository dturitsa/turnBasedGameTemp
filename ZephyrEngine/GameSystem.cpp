#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

void GameSystem::startSystemLoop() {
	while (true) {
		// Main Game Loop


		// Load Scene

		// while 1 & game interval is up
		/*

		for (GameObject obj : currObjects) {
		for (Component c : obj.components) {
		p.push(c.earlyUpdate());
		}
		}

		// wait for all threads to complete

		for (GameObject obj : currObjects) {
		for (Component c : obj.components) {
		p.push(c.update());
		}
		}

		// wait for all threads to complete

		for (GameObject obj : currObjects) {
		for (Component c : obj.components) {
		p.push(c.lateUpdate());
		}
		}

		*/
	}
}

void GameSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	default:
		break;
	}
}