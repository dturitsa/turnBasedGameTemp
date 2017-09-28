#include "GameSystem.h"

GameSystem::GameSystem(MessageBus* mbus) : System(mbus) {

}


GameSystem::~GameSystem() {
}

void GameSystem::startSystemLoop() {
	while (true) {
		// Main Game Loop
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