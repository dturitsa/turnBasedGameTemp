#pragma once
#include "Msg.h"
#include "MessageBus.h"

class System {
public:
	System();
	~System();

	void handleMessage(Msg *msg);

private:

	MessageBus *msgBus;
	//// Usage: msgBus->postMessage(msg);
};


