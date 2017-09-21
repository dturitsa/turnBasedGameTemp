#include "Main.h"

int main(int argc, char *argv[]) { 
	// entry point
	// create ONE message bus that goes to ALL the systems
	MessageBus* mbus = new MessageBus();

	// ex: creating a basic system
	/*
	
	SomeSystem* sysOne = new SomeSystem(mbus); // note that SomeSystem must extend from System. Pass in the message bus we created so that they're all using the same bus
	mbus->someSystem = sysOne;	// add the system to the bus so that the bus has a reference to it
	
	// thread the system
	// start the system
	// repeat for each required system
	*/


}