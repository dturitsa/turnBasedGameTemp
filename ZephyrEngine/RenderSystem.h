#pragma once
#include "System.h"
#include "GameObject.h"
#include <vector>


class RenderSystem : public System {
public:
	RenderSystem(MessageBus* mbus);
	~RenderSystem();

	void handleMessage(Msg * msg);
	void startSystemLoop();

private:

	void renderAllItems();

	std::vector<GameObject> gameObjectsToRender;
};