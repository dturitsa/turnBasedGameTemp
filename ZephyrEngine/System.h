#pragma once
#include "Msg.h"
#include "MessageBus.h"

class MessageBus;

class System {
public:
	System(MessageBus* mbus);
	~System();

	virtual void handleMessage(Msg *msg);

protected:
	MessageBus* msgBus;

private:

	
};

