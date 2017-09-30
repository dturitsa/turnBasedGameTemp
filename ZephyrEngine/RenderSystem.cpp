#include "RenderSystem.h"

RenderSystem::RenderSystem(MessageBus* mbus) : System (mbus) {
	
}


RenderSystem::~RenderSystem() {
}


void RenderSystem::renderAllItems() {
	// temporary system: just prints out the data for each item
	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> data = split(*s, ',');
		std::cout << *s << "\n"; 
	}
	
}

void RenderSystem::startSystemLoop() {
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;
	
	while (true) {
		thisTime = clock();
		if ((thisTime - lastTime) > 100) {
			lastTime = thisTime;
			mtx.lock();
			renderAllItems();
			mtx.unlock();
		}
		// insert framerate lock here
	}
}

void RenderSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case UPDATE_OBJECT_POSITION:
		mtx.lock();
		updateObjPosition(msg);
		mtx.unlock();
		break;
	case NEW_OBJECT_TO_RENDER:
		// parse data from msg to add obj to list
		mtx.lock();
		addObjectToRenderList(msg);
		mtx.unlock();
		break;
	case REMOVE_OBJECT_FROM_RENDER:

		break;
	default:
		break;
	}
}

void RenderSystem::addObjectToRenderList(Msg* m) {
	gameObjectsToRender.push_back(&m->data);
}

void RenderSystem::updateObjPosition(Msg* m) {
	std::vector<std::string> data = split(m->data, ',');
	
	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		
		// found the obj
		if (obj.front() == data.front()) {
			// replace this string's information with new information
			*s = m->data;
			return;
		}

	}
}