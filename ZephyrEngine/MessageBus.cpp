#include "MessageBus.h"


MessageBus::MessageBus() {
}


MessageBus::~MessageBus() {
}

void MessageBus::postMessage(Msg* m) {
	// send the message to every system so that they can all see it
	for (System* s : systemList) {
		s->handleMessage(m);
	}
}

// Used to remove a system from the list
void MessageBus::addSystem(System* s) {
	systemList.push_back(s);
}
