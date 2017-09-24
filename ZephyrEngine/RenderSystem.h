#pragma once
#include "System.h"
#include "ConsoleRenderer.h"
#include <mutex>

class RenderSystem : public System {
public:
	RenderSystem(MessageBus* mbus);
	~RenderSystem();

	void handleMessage(Msg * msg);

private:
	Renderer* rendererPointer;
	RenderObj* b;
	RenderObj* c;
	RenderObj* d;
	RenderObj* a;

	std::mutex renderLock;
};