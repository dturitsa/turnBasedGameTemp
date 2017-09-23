#pragma once
#include "System.h"



System::System(MessageBus* mbus) {
	msgBus = mbus;
}


System::~System() {
}

void System::handleMessage(Msg *msg) {
	switch (msg->type) {
		//// Example
		//case Msg::OPEN_INVENTORY:
		//    break;
	default:
		break;
	}
}