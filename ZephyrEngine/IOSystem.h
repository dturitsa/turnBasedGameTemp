#pragma once
#include "System.h"
#include <Windows.h>
#include <iostream>

class IOSystem : public System {
public:
	IOSystem(MessageBus* mbus);
	~IOSystem();

	void checkKeyPresses();

	void handleMessage(Msg * msg);

private:

};