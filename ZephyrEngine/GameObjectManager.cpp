#include "GameObjectManager.h"



GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}

void GameObjectManager::gameObjectRemoved(GameObject* g) {
	gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), g), gameObjects.end());
	Msg* m = new Msg(GO_REMOVED, g->id);
	msgBus->postMessage(m, this);
}
