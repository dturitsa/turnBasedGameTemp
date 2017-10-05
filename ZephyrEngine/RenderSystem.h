#pragma once
#include "System.h"
#include "GameObject.h"
#include <mutex>

class RenderSystem : public System {
public:
	RenderSystem(MessageBus* mbus);
	~RenderSystem();

	void handleMessage(Msg * msg);
	void startSystemLoop();

	int timeFrame = 10;
private:
	std::mutex mtx;
	void renderAllItems();

	std::vector<std::string*> gameObjectsToRender;
	void addObjectToRenderList(Msg* m);
	void removeObjectFromRenderList(Msg* m);
	void updateObjPosition(Msg* m);
};