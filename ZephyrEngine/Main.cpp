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
	
	// SPECIAL CASE: NEEDS OWN THREAD

	IOSystem* ios = new IOSystem(mbus);
	mbus->addSystem(ios);
	std::thread ioThread(startIOSystem, ios);

	// SPECIAL CASE: NEEDS OWN THREAD
	RenderSystem* rs = new RenderSystem(mbus);
	mbus->addSystem(rs);
	std::thread renderThread(startRenderSystem, rs);

	// SPECIAL CASE: NEEDS OWN THREAD
	GameSystem* gs = new GameSystem(mbus);
	mbus->addSystem(gs);
	std::thread gameSystemThread(startGameSystem, gs);

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
	
		
		//framerate locking testing stuff
		/*
		thisTime = clock();
		if ((thisTime - lastTime) > 100) {
			lastTime = thisTime;		
			p.push(&postMessage, checkKeyPresses);
			p.push(&postMessage, renderFrames);
		}*/
		


	}
	
	//////////////////////////////////////////////////////////////////
	//						Thread Joining							//
	//////////////////////////////////////////////////////////////////
	ioThread.join();
	renderThread.join();
	gameSystemThread.join();
}

// note: Must have "int id" for functinos that are to be run in worker threads
// the id is the thread ID - required for the pooling library
void postMessage(int id, Msg* msg) {
	mbus->postMessage(msg);
}

// Required wrapper functions to thread the calls
void startIOSystem(IOSystem* s) {
	s->startSystemLoop();
}

void startRenderSystem(RenderSystem* s) {
	s->startSystemLoop();
}

void startGameSystem(GameSystem* s) {
	s->startSystemLoop();
}