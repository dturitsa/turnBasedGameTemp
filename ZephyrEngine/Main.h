#pragma once
#include "System.h"
#include <iostream>
#include "RenderSystem.h"
#include "IOSystem.h"

int main(int argc, char *argv[]);

MessageBus* mbus;

void postMessage(Msg* msg) {
	mbus->postMessage(msg);
}
