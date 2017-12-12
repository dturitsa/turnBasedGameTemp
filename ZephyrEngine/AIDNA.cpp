#include "AIDNA.h"


using namespace std;
AIDNA::AIDNA()
{
	
}

AIDNA::AIDNA(map <string, string> paramsMap) {
	rating = stoi(paramsMap.find("rating")->second);
	raycastLength = stof(paramsMap.find("raycastLength")->second);
	raycastAngle = stof(paramsMap.find("raycastAngle")->second);
	avoidanceLockTime = stof(paramsMap.find("avoidanceLockTime")->second);
}

AIDNA::~AIDNA()
{
}

//returns a copy of itself with mutated  values
AIDNA* AIDNA::asexualReproduction(float mutationRate) {
	AIDNA* child = new AIDNA();
	child->rating = rating;
	child->raycastLength = raycastLength;
	child->raycastAngle = raycastAngle;
	child->avoidanceLockTime = avoidanceLockTime;
	child->seekDistance = seekDistance;
	child->engageDistance = engageDistance;

	child->mutate(mutationRate);
	return child;
}

//returns a new AIDNA object pointer created by mixing and randomizing the values of 2 existing AIDNA objects
AIDNA* AIDNA::sexualReproduction(float mutationRate, AIDNA* partner) {
	//rnd number between 0 and 1
	//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return this;
}

//randomizes the variable values in the object. 
//mutation rate should be betweeb 0 and 1;
void AIDNA::mutate(float mutationRate) {
	float rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate / 2.0f - rnd + 1;
	raycastLength *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate/2.0f - rnd + 1;
	raycastAngle *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate / 2.0f - rnd + 1;
	avoidanceLockTime *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate / 2.0f - rnd + 1;
	seekDistance *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate / 2.0f - rnd + 1;
	engageDistance *= rnd;
	if (engageDistance > seekDistance) {
		engageDistance = seekDistance + .1f;
	}
}

string AIDNA::toString() {
	string output = "\nrating: " + to_string(rating) + ",";
	output += "\nraycastLength: " + to_string(raycastLength) + ",";
	output += "\nraycastAngle: " + to_string(raycastAngle) + ",";
	output += "\navoidanceLockTime: " + to_string(avoidanceLockTime) + ",";
	output += "\nseekDistance: " + to_string(seekDistance) + ",";
	output += "\nengageDistance: " + to_string(engageDistance) + ",";

	return output;
}


