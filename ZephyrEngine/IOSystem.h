#pragma once
#include "System.h"
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <thread>

class IOSystem : public System {
public:
	IOSystem(MessageBus* mbus);
	~IOSystem();

	void checkKeyPresses();
	void startSystemLoop();
	void handleMessage(Msg * msg);

	Msg* m;
	
	int timeFrame = 10;
private:

};
