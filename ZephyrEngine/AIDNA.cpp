#include "AIDNA.h"


using namespace std;
AIDNA::AIDNA()
{
	srand(time(NULL));
}

AIDNA::AIDNA(map <string, string> paramsMap) {
	rating = stoi(paramsMap.find("rating")->second);
	raycastLength = stoi(paramsMap.find("raycastLength")->second);
	raycastAngle = stoi(paramsMap.find("raycastAngle")->second);
	avoidanceLockTime = stoi(paramsMap.find("avoidanceLockTime")->second);
}

AIDNA::~AIDNA()
{
}

//returns a copy of itself with mutated  values
AIDNA* AIDNA::asexualReproduction(float mutationRate) {
	return this;
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
	rnd = mutationRate / 2 - rnd + 1;
	raycastLength *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate/2 - rnd + 1;
	raycastAngle *= rnd;

	rnd = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / mutationRate));
	rnd = mutationRate / 2 - rnd + 1;
	avoidanceLockTime *= rnd;
}

float raycastLength = 100;
float raycastAngle = 10;
float avoidanceLockTime = 40;

string AIDNA::toString() {
	string output = "\nrating: " + to_string(rating) + ",";
	output += "\nraycastLength: " + to_string(raycastLength) + ",";
	output += "\nraycastAngle: " + to_string(raycastAngle) + ",";
	output += "\navoidanceLockTime: " + to_string(avoidanceLockTime) + ",";

	return output;
}


