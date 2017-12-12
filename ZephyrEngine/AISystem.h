#pragma once
#include "System.h"
#include "AIObject.h"
#include "AIData.h"
#include "AIDNA.h"
#include "Util.h"
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
	void saveDnaToFile();
	void loadDnaFromFile();

	int timeFrame = 100;//length of a single time frame in milliseconds
	unsigned int frameCount = 0; //number of AI frames that have passed

	//std::mutex mtx;
	AIData aiData;

	std::vector<AIObject*> AIObjects;
	std::vector<AIDNA*> dnaVector;
};

