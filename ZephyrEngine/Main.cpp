#include "Main.h"

int main(int argc, char *argv[]) { 	
	//////////////////////////////////////////////////////////////////
	//						Loading Core							//
	//////////////////////////////////////////////////////////////////
	
	// create ONE message bus that goes to ALL the systems
	mbus = new MessageBus();

	//////////////////////////////////////////////////////////////////
	//						SYSTEM CREATION							//
	// DO NOT START SYSTEM LOOPS IN HERE (if a loop is required)
	//////////////////////////////////////////////////////////////////	
	IOSystem* ios = new IOSystem(mbus);
	mbus->addSystem(ios);
	
	RenderSystem* rs = new RenderSystem(mbus);
	mbus->addSystem(rs);

	GameSystem* gs = new GameSystem(mbus);
	mbus->addSystem(gs);

	PhysicsSystem* ps = new PhysicsSystem(mbus);
	mbus->addSystem(ps);

	AISystem* ais = new AISystem(mbus);
	mbus->addSystem(ais);

	std::cout << "All systems created";

	//////////////////////////////////////////////////////////////////
	//						Loading Config							//
	//////////////////////////////////////////////////////////////////
	//int numberOfWorkerThreads = 16; // Default to 16

	std::string rawConfigData = openFileRemoveSpaces("config.txt");
	
	// Temporary config file is structured to only list the time frames for
	// GameSystem, RenderThread, and ioThread
	std::vector<std::string> configData = split(rawConfigData, ',');
	std::string::size_type sz;

	//gs->timeFrame = std::stoi(configData.at(0), &sz);
	//rs->timeFrame = std::stoi(configData.at(1), &sz);
	//ios->timeFrame = std::stoi(configData.at(2), &sz);
//	ps->timeFrame = std::stoi(configData.at(3), &sz);

	// Not using this right now, move it to game system/Render/Physics later maybe
	//// Create worker thread pool
	//ctpl::thread_pool p(numberOfWorkerThreads); // number of threads in pool
	//											// for p.push usage, see the ctpl_stl.h header file

	//////////////////////////////////////////////////////////////////
	//				Start Core System Threads     					//
	//////////////////////////////////////////////////////////////////
	std::thread gameSystemThread(startGameSystem, gs);
	std::thread physicsThread(startPhysicsSystem, ps);
	std::thread renderThread(startRenderSystem, rs);
	std::thread ioThread(startIOSystem, ios);
	std::thread aiThread(startAISystem, ais);
	


	//////////////////////////////////////////////////////////////////
	//						Console Loop							//
	//////////////////////////////////////////////////////////////////
	bool alive = true; //Move this
	clock_t thisTime = clock();
	int currentGameTime = 0;

	// TO DO: Implement 
	while (alive) {
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}
		currentGameTime = thisTime + 100;

		SDL_Event windowEvent;
		while (SDL_PollEvent(&windowEvent)) {
			if (SDL_QUIT == windowEvent.type) {
				rs->stopSystemLoop();
				gs->alive  = false;
				ps->alive = false;
				ios->alive = false;
				ais->alive = false;
				alive = false;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////
	//						Thread Joining							//
	//////////////////////////////////////////////////////////////////
	ioThread.join();
	physicsThread.join();
	renderThread.join();
	gameSystemThread.join();
	

	return 1;
}

// note: Must have "int id" for functinos that are to be run in worker threads
// the id is the thread ID - required for the pooling library
void postMessage(int id, Msg* msg) {
	//mbus->postMessage(msg);
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

void startPhysicsSystem(PhysicsSystem* s) {
	s->startSystemLoop();
}

void startAISystem(AISystem* s) {
	s->startSystemLoop();
}