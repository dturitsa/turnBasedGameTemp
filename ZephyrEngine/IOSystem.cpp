#include "IOSystem.h"


IOSystem::IOSystem(MessageBus* mbus) : System(mbus) {
	m = new Msg(EMPTY_MESSAGE, "");
}


IOSystem::~IOSystem() {
}

void IOSystem::startSystemLoop() {
	// used to prevent the io system from posting messages too often
	clock_t thisTime = clock();

	int currentGameTime = 0;
	while (true) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}

		currentGameTime = thisTime + timeFrame;

		//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
		//OutputDebugString("IO Loop on thread: ");
		//OutputDebugString(s.c_str());
		//OutputDebugString("\n");

		checkKeyPresses();


	}
}

// checks keypresses
void IOSystem::checkKeyPresses() {
	m->data = "";
	

	// Need to decide how we want to do this section - might be some issues here
	// with regard to pressing multiple keys at the same time
	// A possible way to do it is to send data instead of posting messages for 
	// specific keys, and then having another function somewhere that
	// parses the data to get the proper code to the Gamesystem but that's
	// a bit convoluted
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		OutputDebugString("Up Pressed\n");
		m->type = UP_ARROW_PRESSED;
		if ((clock() - keyspressed["up"]) > timebetweenPresses) {
			keyspressed["up"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		OutputDebugString("Down Pressed\n");
		m->type = DOWN_ARROW_PRESSED;
		if ((clock() - keyspressed["down"]) > timebetweenPresses) {
			keyspressed["down"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		OutputDebugString("Space Pressed\n");
		m->type = SPACEBAR_PRESSED;
		if ((clock() - keyspressed["space"]) > timebetweenPresses) {
			keyspressed["space"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		OutputDebugString("Right Pressed\n");
		m->type = RIGHT_ARROW_PRESSED;
		if ((clock() - keyspressed["right"]) > timebetweenPresses) {
			keyspressed["right"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		OutputDebugString("Left Pressed\n");
		m->type = LEFT_ARROW_PRESSED;
		if ((clock() - keyspressed["left"]) > timebetweenPresses) {
			keyspressed["left"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		OutputDebugString("A Pressed\n");
		m->type = KEY_A_PRESSED;
		if ((clock() - keyspressed["a"]) > timebetweenPresses) {
			keyspressed["a"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		OutputDebugString("D Pressed\n");
		m->type = KEY_D_PRESSED;
		if ((clock() - keyspressed["d"]) > timebetweenPresses) {
			keyspressed["d"] = clock();
			msgBus->postMessage(m, this);
		}
	}

	if (GetAsyncKeyState('Z') & 0x8000) {
		OutputDebugString("Z Pressed\n");
		m->type = TEST_KEY_PRESSED;
		if ((clock() - keyspressed["z"]) > timebetweenPresses) {
			keyspressed["z"] = clock();
			msgBus->postMessage(m, this);
		}
	}
}




// Currently the IO system doesn't really do anything other than check IO, there should be no messages for it to handle
// theorietically the IO system jsut posts messages
void IOSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case CHECK_KEY_PRESSES:
		checkKeyPresses();
		break;
	default:
		break;
	}
}