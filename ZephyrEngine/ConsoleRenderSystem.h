#pragma once
#include "System.h"
#include "ConsoleRenderer.h"
#include <mutex>

class ConsoleRenderSystem : public System {
public:
	ConsoleRenderSystem(MessageBus* mbus);
	~ConsoleRenderSystem();

	void handleMessage(Msg * msg);
	void startSystemLoop();
private:
	Renderer* rendererPointer;
	RenderObj* b;
	RenderObj* c;
	RenderObj* d;
	RenderObj* a;

	std::mutex renderLock;
};