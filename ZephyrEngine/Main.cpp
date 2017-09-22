#include "Main.h"
#include "ctpl_stl.h"
#include "ConsoleRenderer.h"

// insert game object list here
// List<GameObject> currObjects;

int main(int argc, char *argv[]) { 
	// Create worker thread pool
	ctpl::thread_pool p(16); // number of threads in pool
	// for p.push usage, see the ctpl_stl.h header file


	// create ONE message bus that goes to ALL the systems
	MessageBus* mbus = new MessageBus();

	// create IO system

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
	
	
	//creates a new renderer object to handle the console display
	Renderer* rendererPointer = new Renderer();

	//creates some render objects
	RenderObj* b = new RenderObj('b', 3, 1);
	rendererPointer->addRenderObj(b);

	RenderObj* c = new RenderObj('c', 1, 4);
	rendererPointer->addRenderObj(c);

	RenderObj* d = new RenderObj('d', 7, 7);
	rendererPointer->addRenderObj(d);

	RenderObj* a = new RenderObj('^', 1, 1);
	rendererPointer->addRenderObj(a);

	//the temporary game loop
	while (1) {
		//tell the renderer to render the next frame and wait for a bit
		rendererPointer->renderFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//update the renderObj between frames and the renderer will update them as well
		if (GetKeyState('W') & 0x8000) {
			int temp = a->getX();
			temp--;
			a->setX(temp);
		}

		if (GetKeyState('S') & 0x8000) {
			int temp = a->getX();
			temp++;
			a->setX(temp);
		}

		if (GetKeyState('A') & 0x8000) {
			int temp = a->getY();
			temp--;
			a->setY(temp);
		}

		if (GetKeyState('D') & 0x8000) {
			int temp = a->getY();
			temp++;
			a->setY(temp);
		}
	}



	/*
	
	SomeSystem* sysOne = new SomeSystem(mbus); // note that SomeSystem must extend from System. Pass in the message bus we created so that they're all using the same bus
	mbus->someSystem = sysOne;	// add the system to the bus so that the bus has a reference to it
	
	*/


}