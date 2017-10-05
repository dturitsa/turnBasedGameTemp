#include "Main.h"

int main(int argc, char *argv[]) { 	
	//////////////////////////////////////////////////////////////////
	//						Loading Config							//
	//////////////////////////////////////////////////////////////////
	int numberOfWorkerThreads = 16; // Default to 16
	


	//////////////////////////////////////////////////////////////////
	//						Loading Core							//
	//////////////////////////////////////////////////////////////////
	
	// create ONE message bus that goes to ALL the systems
	mbus = new MessageBus();
	
	// Create worker thread pool
	ctpl::thread_pool p(numberOfWorkerThreads); // number of threads in pool
												// for p.push usage, see the ctpl_stl.h header file

	//////////////////////////////////////////////////////////////////
	//						SYSTEM CREATION							//
	//////////////////////////////////////////////////////////////////
	
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
	//						Console Loop							//
	//////////////////////////////////////////////////////////////////

	// TO DO: Implement 
	while (true) {
		
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