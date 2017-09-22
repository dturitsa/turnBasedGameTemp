#pragma once
#include "Msg.h"
#include "MessageBus.h"

class MessageBus;

class System {
public:
	System(MessageBus* mbus);
	~System();

	void handleMessage(Msg *msg);

private:

	MessageBus* msgBus;
	//// Usage: msgBus->postMessage(msg);
};

