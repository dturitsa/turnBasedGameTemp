#include "IOSystem.h"


IOSystem::IOSystem(MessageBus* mbus) : System(mbus) {
	m = new Msg(EMPTY_MESSAGE, "");
}


IOSystem::~IOSystem() {
}

void IOSystem::startSystemLoop() {
	// used to prevent the io system from posting messages too often
	clock_t thisTime = clock();

	while (true) {
		thisTime = clock();
		if (thisTime  < timeFrame) {
			Sleep(timeFrame - thisTime);
		}
		timeFrame += 10;

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
	if (GetKeyState(VK_UP) & 0x8000) {
		OutputDebugString("Up Pressed\n");
		m->type = UP_ARROW_PRESSED;
		msgBus->postMessage(m, this);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		OutputDebugString("Down Pressed\n");
		m->type = DOWN_ARROW_PRESSED;
		msgBus->postMessage(m, this);
	}

	if (GetKeyState(VK_SPACE) & 0x8000) {
		OutputDebugString("Space Pressed\n");
		m->type = SPACEBAR_PRESSED;
		msgBus->postMessage(m, this);
	}

	if (GetKeyState(VK_RIGHT) & 0x8000) {
		OutputDebugString("Right Pressed\n");
		m->type = RIGHT_ARROW_PRESSED;
		msgBus->postMessage(m, this);
	}

	if (GetKeyState(VK_LEFT) & 0x8000) {
		OutputDebugString("Left Pressed\n");
		m->type = LEFT_ARROW_PRESSED;
		msgBus->postMessage(m, this);
	}

	if (GetAsyncKeyState('Z') & 0x8000) {
		OutputDebugString("Z Pressed\n");
		m->type = TEST_KEY_PRESSED;
		msgBus->postMessage(m, this);
	}

	/*
	if (m->type != EMPTY_MESSAGE) {
		msgBus->postMessage(m);
		m->type = EMPTY_MESSAGE;
		
	}
	*/

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