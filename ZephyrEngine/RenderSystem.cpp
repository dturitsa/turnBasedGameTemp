#include "RenderSystem.h"


RenderSystem::RenderSystem(MessageBus* mbus) : System(mbus) {
	rendererPointer = new Renderer();

	// for the sake of testing I have inserted the render objects directly into the class as properties
	b = new RenderObj('b', 3, 1);
	c = new RenderObj('c', 1, 4);
	d = new RenderObj('d', 7, 7);
	a = new RenderObj('^', 1, 1);
	rendererPointer->addRenderObj(b);
	rendererPointer->addRenderObj(c);
	rendererPointer->addRenderObj(d);
	rendererPointer->addRenderObj(a);
}


RenderSystem::~RenderSystem() {
}

// Render system must not be rendering over itself. Therefore, use a mutex to for it to do once at a time.

void RenderSystem::handleMessage(Msg *msg) {
	renderLock.lock();
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case RENDER_FRAME_TEST:
		rendererPointer->renderFrame();
		break;
	case UPDATE_TEST_OBJ:
		// based on which data strng we got (representing direction to move), we update the a obj
		// Normally this wouldn't be done in the render system class but we dont have the other systems set up yet
		if ((msg->data).find("W") != std::string::npos) {
			// W was pressed
			int temp = a->getX();
			temp--;
			a->setX(temp);
		}

		if ((msg->data).compare("S") != std::string::npos) {
			// S was pressed
			int temp = a->getX();
			temp++;
			a->setX(temp);
		}

		if ((msg->data).compare("A") != std::string::npos) {
			// A was pressed
			int temp = a->getY();
			temp--;
			a->setY(temp);
		}

		if ((msg->data).compare("D") != std::string::npos) {
			// D was pressed
			int temp = a->getY();
			temp++;
			a->setY(temp);
		}

		if ((msg->data).compare("Z") != std::string::npos) {
			// Z was pressed
			a->setX(19);
			a->setY(19);
		}

		std::cout << "A POSITION Y AND X: " << a->getY() << " : " << a->getX();
		std::cout << "\nKEYS BEING PRESSED: " << msg->data;
		break;
	default:
		break;
	}

	renderLock.unlock();
}