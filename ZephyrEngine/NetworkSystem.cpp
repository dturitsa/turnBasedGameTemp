#include "NetworkSystem.h"



NetworkSystem::NetworkSystem(MessageBus* mbus) : System(mbus) {
	m = new Msg(EMPTY_MESSAGE, "");
}

NetworkSystem::~NetworkSystem()
{
}

void NetworkSystem::startSystemLoop() {
	// used to prevent the io system from posting messages too often
	clock_t thisTime = clock();

	int currentGameTime = 0;
	int timerGameTime = 0;

	while (alive) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}

		// if testing mode
		if (echoMode) {

			if (thisTime >= timerGameTime) { // time is up
				// send stuff in turns
				broadcastTurnInfo();

				// set the new turn timer
				timerGameTime = thisTime + turnTimer;
			}
		}

		currentGameTime = thisTime + timeFrame;
	}
}

void NetworkSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
		case NETWORK_S_IDLE:
			playerTurnAction[actionCounter] = "NETWORK_S_IDLE";
			playerTurnTarget[actionCounter] = "A0"; // can be changed to use -- later
			// always add 1 to the action counter
			actionCounter++;
			break;
	default:
		break;
	}
}

void NetworkSystem::aggregateTurnInfo(Msg* m) {

}

void NetworkSystem::broadcastTurnInfo() {
	std::string turnInfo = "";

	if (echoMode) {
		// needs to be changed later to use a loop in case we change max action count
		turnInfo =
			playerID + "," + playerTurnAction[0] + "," + playerTurnTarget[0] + "]D1, NETWORK_R_IDLE, A0]D2, NETWORK_R_IDLE, A0]D3, NETWORK_R_IDLE, A0]\n" +
			playerID + "," + playerTurnAction[1] + "," + playerTurnTarget[1] + "]D1, NETWORK_R_IDLE, A0]D2, NETWORK_R_IDLE, A0]D3, NETWORK_R_IDLE, A0]\n" +
			playerID + "," + playerTurnAction[2] + "," + playerTurnTarget[2] + "]D1, NETWORK_R_IDLE, A0]D2, NETWORK_R_IDLE, A0]D3, NETWORK_R_IDLE, A0]\n" +
			playerID + "," + playerTurnAction[3] + "," + playerTurnTarget[3] + "]D1, NETWORK_R_IDLE, A0]D2, NETWORK_R_IDLE, A0]D3, NETWORK_R_IDLE, A0]";
	}

	// im being lazy here and just sending out the string since ideally the network class doesn't know how to parse. alternatively i can parse here, depends on
	// what we decide later
	Msg* tm = new Msg(NETWORK_TURN_BROADCAST, turnInfo);
	msgBus->postMessage(tm);

	// reset the action counter when we broadcast 
	actionCounter = 0;
}