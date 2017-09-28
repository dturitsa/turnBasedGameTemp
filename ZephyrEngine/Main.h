#pragma once
#include "System.h"
#include <iostream>
#include "ConsoleRenderSystem.h"
#include "RenderSystem.h"
#include "GameSystem.h"
#include "IOSystem.h"
#include "ctpl_stl.h"
#include <time.h>

MessageBus* mbus;

int main(int argc, char *argv[]);

void postMessage(int id, Msg* msg);