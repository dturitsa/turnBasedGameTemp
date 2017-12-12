#include "AISystem.h"

using namespace std;

AISystem::AISystem(MessageBus* mbus) : System(mbus) {
	aiData = *(new AIData());

	//create randomized dnaData
	//for (int i = 0; i < 5; i++) {
	//	AIDNA* d = new AIDNA();
	//	d->mutate(.5);
	//	dnaVector.push_back(d);
	//}
	loadDnaFromFile();
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
			a->pos.x = stof(data[2]);
			a->pos.y = stof(data[3]);
			a->orientation = stoi(data[5]);
			a->aiData = &aiData;
			
			//a->dna->mutate(.5f);

			AIDNA* bestDna = dnaVector[0];
			int bestRating = -999999;
			for (AIDNA* dna : dnaVector) {
				if (dna->rating > bestRating && dna->inUse < 1) {
					bestRating = dna->rating;
					bestDna = dna;
				}
			}

			a->dna = bestDna;
			bestDna->inUse++;
			AIObjects.push_back(a);
		}

		//check if an object if of interest to the AI system and add it to the world objects vector
		if (atof(data[8].c_str()) == 1 && data[9] != "Cannonball") {
			WorldObject* w = new WorldObject();
			w->id = data[0];
			w->pos.x = stof(data[2]);
			w->pos.y = stof(data[3]);
			w->orientation = stoi(data[5]);
			aiData.worldObjects.push_back(w);
		}
		break;
	}
	case UPDATE_OBJECT_POSITION: {
		data = split(msg->data, ',');

		for (AIObject* a : AIObjects) {
			if (a->id == data[0]) {
				a->pos.x = stof(data[2]);
				a->pos.y = stof(data[3]);
				a->orientation = stoi(data[5]);
			}
		}

		for (WorldObject* w : aiData.worldObjects) {
			if (w->id == data[0]) {
				w->pos.x = stof(data[2]);
				w->pos.y = stof(data[3]);
				w->orientation = stoi(data[5]);
				w->width = stoi(data[6]);
				w->height = stoi(data[7]);
			}
		}
		break;
	}
	case SCORED_HIT: {
		for (AIObject* a : AIObjects) {
			if (a->id == data[0]) {
				a->scoredHit(data[1], data[2]);
					continue;
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
				toEraseA->dna->inUse--;
				toEraseA->dna->rating -= 500;
				OutputDebugString("\n\nDEATH PENALTY\n\n");
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
		frameCount++;

		handleMsgQ();

		//update AI state
		for (AIObject* a : AIObjects) {
			a->update();
		}


		for (WorldObject* w : aiData.worldObjects) {
			float halfWidth = w->width / 2.0f;
			float halfHeight = w->height / 2.0f;
			// Upper Left
			w->c[0].x = w->pos.x - halfWidth;
			w->c[0].y = w->pos.y + halfHeight;
			w->c[0].rotateFromOrigin((float)w->pos.x, (float)w->pos.y, (float)w->orientation);

			//Upper Right
			w->c[1].x = w->pos.x + halfWidth;
			w->c[1].y = w->pos.y + halfHeight;
			w->c[1].rotateFromOrigin((float)w->pos.x, (float)w->pos.y, (float)w->orientation);

			// Bottom Right
			w->c[2].x = w->pos.x + halfWidth;
			w->c[2].y = w->pos.y - halfHeight;
			w->c[2].rotateFromOrigin((float)w->pos.x, (float)w->pos.y, (float)w->orientation);

			// Bottom Left
			w->c[3].x = w->pos.x - halfWidth;
			w->c[3].y = w->pos.y - halfHeight;
			w->c[3].rotateFromOrigin((float)w->pos.x, (float)w->pos.y, (float)w->orientation);
		}

		//loop through list of messages to send that were added by AI objects
		for each (Msg* m in aiData.toPostVector) {
			msgBus->postMessage(m, this);
		}
		aiData.toPostVector.clear();

		//update dna data for learning. this is spread accross several frames to avoid framerate drops

		//create children for successful dna combinations, and kill unsuccessful ones
		if (frameCount % 100 == 0) {
			
			int bestRating = -999999;
			int worstRating = 999999;
			AIDNA* bestDna = dnaVector[0];
			AIDNA* worstDna = dnaVector[0];
			for (AIDNA* dna : dnaVector) {
				if (dna->rating > bestRating) {
					bestRating = dna->rating;
					bestDna = dna;
				}
				if (dna->rating < worstRating && dna->inUse < 1) {
					worstRating = dna->rating;
					worstDna = dna;
				}
			}
			// create a child of the best rated dna combination in the group
			dnaVector.push_back(bestDna->asexualReproduction(.5));

			//kill the worst rated dna combination
			dnaVector.erase(remove(dnaVector.begin(), dnaVector.end(), worstDna), dnaVector.end());

		}

		//compensates for dna rating drift.
		else if (frameCount % 102 == 0) {
			int averageRating = 0;

			for (AIDNA* dna : dnaVector) {
				averageRating += dna->rating;
			}
			averageRating /= dnaVector.size();

			for (AIDNA* dna : dnaVector) {
				dna->rating -= averageRating;
			}
		}

		//save the updated dna configurations to file
		else if (frameCount % 103 == 0) {
			saveDnaToFile();
		}
	}
}

void AISystem::saveDnaToFile() {
	//string saveData = to_string(frameCount);
	string output = "";
	for (AIDNA* dna : dnaVector) {
		output += dna->toString();
		output.pop_back();//remove the trailing ','
		output += ";\n";
	}
	writeToFile("dnaData.txt", output);
}

void AISystem::loadDnaFromFile() {
	std::string data = openFileRemoveSpaces("dnaData.txt");

	vector<string> splitDataVector = split(data, ';');//split gameobjects by

	//AIDNA* dna; //new dna object to be created

	for (unsigned int j = 0; j < splitDataVector.size(); j++) {
		//AIDNA*  dna;
		vector<string> splitObjData = split(splitDataVector[j], ',');

		map<string, string> dnaDataMap;
		//loop through elements of each dna object and add them to the object parameter map
		for (unsigned int i = 0; i < splitObjData.size(); i++) {
			vector<string> keyValue = split(splitObjData[i], ':');
			dnaDataMap[keyValue[0]] = keyValue[1];
		}
		dnaVector.push_back(new AIDNA(dnaDataMap));
	}
}

