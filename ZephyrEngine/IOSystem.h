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
	
	const int timeFrame = 10;

	std::map<std::string, clock_t> keyspressed;

	const int timebetweenPresses = 150;
private:

};
