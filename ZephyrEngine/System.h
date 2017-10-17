#pragma once
#include "Msg.h"
#include "MessageBus.h"
#include <queue>
#include <mutex>


class MessageBus;

class System {
public:
	System(MessageBus* mbus);
	~System();

	virtual void handleMessage(Msg *msg);
	void addToMsgQ(Msg *msg);
	
protected:
	MessageBus* msgBus;
	std::queue<Msg*> msgQ;
	void handleMsgQ();
	std::mutex mtx;

private:
	
	
};

