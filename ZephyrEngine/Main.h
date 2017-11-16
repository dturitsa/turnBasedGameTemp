#pragma once
#include "System.h"
#include <iostream>
#include "RenderSystem.h"
#include "GameSystem.h"
#include "IOSystem.h"
#include "PhysicsSystem.h"
#include "AISystem.h"
#include "AudioSystem.h"
#include <time.h>


MessageBus* mbus;
volatile bool malive;

int main(int argc, char *argv[]);

void postMessage(int id, Msg* msg);
void startIOSystem(IOSystem* s);
void startRenderSystem(RenderSystem* s);
void startGameSystem(GameSystem* s);
void startPhysicsSystem(PhysicsSystem* s);
void startAISystem(AISystem* s); 
void startAudioSystem(AudioSystem* s);


