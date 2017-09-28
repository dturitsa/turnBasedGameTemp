#include "RenderSystem.h"

RenderSystem::RenderSystem(MessageBus* mbus) : System (mbus) {
	
}


RenderSystem::~RenderSystem() {
}


void RenderSystem::renderAllItems() {
	// probably needs a lock
}

void RenderSystem::startSystemLoop() {
	while (true) {
		renderAllItems();
		// insert framerate lock here
	}
}

void RenderSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	default:
		break;
	}
}