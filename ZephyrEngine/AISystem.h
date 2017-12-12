#pragma once
#include "System.h"
#include "AIObject.h"
#include "AIData.h"
#include "AIDNA.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <thread>

class AISystem : public System
{
public:
	AISystem(MessageBus* mbus);
	void startSystemLoop();
	void handleMessage(Msg *msg);
	int timeFrame = 100;
	//std::mutex mtx;
	AIData aiData;

	std::vector<AIObject*> AIObjects;
};

