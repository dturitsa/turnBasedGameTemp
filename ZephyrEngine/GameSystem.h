#pragma once
#include "System.h"
#include "GameObject.h"
#include "DummyGameObj.h"

class GameSystem : public System {
public:
	GameSystem(MessageBus* mbus);
	~GameSystem();

	void handleMessage(Msg * msg);

	void startSystemLoop();
	void startTestLevel();

	std::vector<GameObject*> gameObjects;
private:
};