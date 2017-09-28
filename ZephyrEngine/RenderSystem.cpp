#include "RenderSystem.h"

RenderSystem::RenderSystem(MessageBus* mbus) : System(mbus) {
	
}


RenderSystem::~RenderSystem() {
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