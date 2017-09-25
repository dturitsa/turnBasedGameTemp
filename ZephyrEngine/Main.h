#pragma once
#include "System.h"
#include <iostream>
#include "RenderSystem.h"
#include "IOSystem.h"
#include "ctpl_stl.h"
#include <time.h>

int main(int argc, char *argv[]);

MessageBus* mbus;

// note: Must have "int id" for functinos that are to be run in worker threads
// the id is the thread ID - required for the pooling library
void postMessage(int id, Msg* msg) {
	mbus->postMessage(msg);
}