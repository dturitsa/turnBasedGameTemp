#include "System.h"

System::System(MessageBus* mbus) {
	msgBus = mbus;
}


System::~System() {
}

void handleMessage(Msg *msg) {
	switch (msg->type) {
		//// Example
		//case Msg::OPEN_INVENTORY:
		//    break;
	}
}
