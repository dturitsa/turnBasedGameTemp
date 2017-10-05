#include "IOSystem.h"


IOSystem::IOSystem(MessageBus* mbus) : System(mbus) {
	m = new Msg(EMPTY_MESSAGE, "");
}


IOSystem::~IOSystem() {
}

void IOSystem::startSystemLoop() {
	// used to prevent the io system from posting messages too often
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;

	while (true) {
		thisTime = clock();
		if ((thisTime - lastTime) > timeFrame) {
			lastTime = thisTime;
			checkKeyPresses();
		}
	}
}

// checks keypresses
void IOSystem::checkKeyPresses() {
	m->data = "";

	/*if (GetKeyState('W') & 0x8000) {
		m->data += "W";
	}

	if (GetKeyState('S') & 0x8000) {
		m->data += "S";
	}

	if (GetKeyState('A') & 0x8000) {
		m->data += "A";
	}

	if (GetKeyState('D') & 0x8000) {
		m->data += "D";
	}*/

	if (GetKeyState('Z') & 0x8000) {
		//m->data += "Z";
		m->type = TEST_KEY_PRESSED;
		msgBus->postMessage(m);
	}

	// msgBus->postMessage(m);
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