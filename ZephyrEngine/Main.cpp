#include "Main.h"


// insert game object list here
// List<GameObject> currObjects;

int main(int argc, char *argv[]) { 
	// create ONE message bus that goes to ALL the systems
	mbus = new MessageBus();
	
	// Create worker thread pool
	ctpl::thread_pool p(16); // number of threads in pool
	// for p.push usage, see the ctpl_stl.h header file


	//////////////////////////////////////////////////////////////////
	//						SYSTEM CREATION							//
	//////////////////////////////////////////////////////////////////

	//creates a new renderer object to handle the console display
	RenderSystem* rs = new RenderSystem(mbus);
	mbus->addSystem(rs);
	
	// create IO system
	IOSystem* ios = new IOSystem(mbus);
	mbus->addSystem(ios);
	
	std::cout << "All systems created";

	Msg* checkKeyPresses = new Msg(CHECK_KEY_PRESSES, "");
	Msg* renderFrames = new Msg(RENDER_FRAME_TEST, "");

	//Framerate locking testing
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;


	// TEMPORARY LOOP FOR TESTING MESSAGE SYSTEM WITH CONSOLE RENDERER
	while (true) {

		// Working on its own thread because we used p.push		
		//p.push(&postMessage, checkKeyPresses);

		// Working on its own thread because we used p.push
		//p.push(&postMessage, renderFrames);

		// note that we can also do mbus->postMessage(renderFrames);
		// postMessage(renderFrames);

		

		
		

		//framerate locking testing stuff
		thisTime = clock();
		if ((thisTime - lastTime) > 100) {
			lastTime = thisTime;			
			mbus->postMessage(renderFrames);
			mbus->postMessage(checkKeyPresses);
		}
		


	}





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
