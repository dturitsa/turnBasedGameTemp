#include "AISystem.h"

using namespace std;

AISystem::AISystem(MessageBus* mbus) : System(mbus) {
	aiData = *(new AIData());

	for (int i = 0; i < 5; i++) {
		AIDNA* d = new AIDNA();
		d->mutate(.5);
		dnaVector.push_back(d);
	}
}

void AISystem::handleMessage(Msg *msg)
{
	System::handleMessage(msg);
	//OutputDebugString("Updating AI object: ");
	//OutputDebugString(data[0].c_str());
	//OutputDebugString("\n");

	vector<std::string> data;

	data = split(msg->data, ',');

	switch (msg->type)
	{
	case GO_ADDED: {

		data = split(msg->data, ',');

		//create new AI objects
		//there should probably be an AI enabled flag. just checking id's for now
		// Hank: Better way to check for enemies, now dynamically spawned enemies get AI too!
		if (data[0].find("enemy") != std::string::npos) {

			AIObject* a = new AIObject();
			a->id = data[0];
			a->pos.x = atof(data[2].c_str());
			a->pos.y = atof(data[3].c_str());
			a->orientation = atof(data[5].c_str());
			a->aiData = &aiData;
			
			//a->dna->mutate(.5f);

			AIDNA* bestDna = dnaVector[0];
			int bestRating = -999999;
			for (AIDNA* dna : dnaVector) {
				if (dna->rating > bestRating) {
					bestRating = dna->rating;
					bestDna = dna;
				}
			}

			a->dna = bestDna;
			
			AIObjects.push_back(a);
		}

		//check if an object if of interest to the AI system and add it to the world objects vector
		if (atof(data[8].c_str()) == 1 && data[9] != "Cannonball") {
			WorldObject* w = new WorldObject();
			w->id = data[0];
			w->pos.x = atof(data[2].c_str());
			w->pos.y = atof(data[3].c_str());
			w->orientation = atof(data[5].c_str());
			aiData.worldObjects.push_back(w);
		}
		break;
	}
	case UPDATE_OBJECT_POSITION: {
		data = split(msg->data, ',');

		for (AIObject* a : AIObjects) {
			if (a->id == data[0]) {
				a->pos.x = atof(data[2].c_str());
				a->pos.y = atof(data[3].c_str());
				a->orientation = atof(data[5].c_str());
			}
		}

		for (WorldObject* w : aiData.worldObjects) {
			if (w->id == data[0]) {
				w->pos.x = atof(data[2].c_str());
				w->pos.y = atof(data[3].c_str());
				w->orientation = atof(data[5].c_str());
				w->width = atof(data[6].c_str());
				w->height = atof(data[7].c_str());
			}
		}
		break;
	}
	case GO_REMOVED: {
		AIObject* toEraseA;
		WorldObject* toEraseW;

		for (AIObject* a : AIObjects) {
			if (a->id == data[0]) {
				toEraseA = a;

			}
		}
		AIObjects.erase(remove(AIObjects.begin(), AIObjects.end(), toEraseA), AIObjects.end());

		for (WorldObject* w : aiData.worldObjects) {
			if (w->id == data[0]) {
				toEraseW = w;
			}

		}
		aiData.worldObjects.erase(remove(aiData.worldObjects.begin(), aiData.worldObjects.end(), toEraseW), aiData.worldObjects.end());
		break;
	}
	default:
		break;
	}

}

void AISystem::startSystemLoop() {
	//clocks for limiting AIloop speed
	clock_t thisTime = clock();

	int currentGameTime = 0;
	while (alive) {
		thisTime = clock();
		if (thisTime < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}
		currentGameTime = thisTime + timeFrame;

		handleMsgQ();

		//update AI state
		for (AIObject* a : AIObjects) {
			a->update();
		}

		for (WorldObject* w : aiData.worldObjects) {
			float halfWidth = w->width / 2;
			float halfHeight = w->height / 2;
			// Upper Left
			w->c[0].x = w->pos.x - halfWidth;
			w->c[0].y = w->pos.y + halfHeight;
			w->c[0].rotateFromOrigin(w->pos.x, w->pos.y, w->orientation);

			//Upper Right
			w->c[1].x = w->pos.x + halfWidth;
			w->c[1].y = w->pos.y + halfHeight;
			w->c[1].rotateFromOrigin(w->pos.x, w->pos.y, w->orientation);

			// Bottom Right
			w->c[2].x = w->pos.x + halfWidth;
			w->c[2].y = w->pos.y - halfHeight;
			w->c[2].rotateFromOrigin(w->pos.x, w->pos.y, w->orientation);

			// Bottom Left
			w->c[3].x = w->pos.x - halfWidth;
			w->c[3].y = w->pos.y - halfHeight;
			w->c[3].rotateFromOrigin(w->pos.x, w->pos.y, w->orientation);
		}

		//loop through list of messages to send that were added by AI objects
		for each (Msg* m in aiData.toPostVector) {
			msgBus->postMessage(m, this);
		}
		aiData.toPostVector.clear();
	}
}

