#include "Main.h"


// insert game object list here
// List<GameObject> currObjects;

int main(int argc, char *argv[]) { 
	// create ONE message bus that goes to ALL the systems
	mbus = new MessageBus();
	
	// Create worker thread pool
	ctpl::thread_pool p(16); // number of threads in pool
	// for p.push usage, see the ctpl_stl.h header file

	// create Permanent thread pools

	//////////////////////////////////////////////////////////////////
	//						SYSTEM CREATION							//
	//////////////////////////////////////////////////////////////////

	// creates a new renderer object to handle the console display
	// note that rendering systems have thier own loop thus is a special case
	ConsoleRenderSystem* crs = new ConsoleRenderSystem(mbus);
	mbus->addSystem(crs);
	
	// create IO system
	IOSystem* ios = new IOSystem(mbus);
	mbus->addSystem(ios);

	// SPECIAL CASE: NEEDS OWN THREAD
	RenderSystem* rs = new RenderSystem(mbus);
	mbus->addSystem(rs);
	std::thread renderThread(rs->startSystemLoop);

	// SPECIAL CASE: NEEDS OWN THREAD
	GameSystem* gs = new GameSystem(mbus);
	mbus->addSystem(gs);
	std::thread gameSystemThread(gs->startSystemLoop);

	std::cout << "All systems created";




	//////////////////////////////////////////////////////////////////
	//						Temporary Game Loop						//
	//////////////////////////////////////////////////////////////////

	Msg* checkKeyPresses = new Msg(CHECK_KEY_PRESSES, "");
	Msg* renderFrames = new Msg(RENDER_FRAME_TEST, "");

	//Framerate locking testing
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;


	// TEMPORARY LOOP FOR TESTING MESSAGE SYSTEM WITH CONSOLE RENDERER
	// Note: Probably will use main thread for console debug/control type system thing later
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
			p.push(&postMessage, checkKeyPresses);
			p.push(&postMessage, renderFrames);
			//mbus->postMessage(renderFrames);
			//mbus->postMessage(checkKeyPresses);
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

// note: Must have "int id" for functinos that are to be run in worker threads
// the id is the thread ID - required for the pooling library
void postMessage(int id, Msg* msg) {
	mbus->postMessage(msg);
}