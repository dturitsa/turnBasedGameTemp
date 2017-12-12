#pragma once
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
	

class AIDNA
{
public:
	AIDNA();
	~AIDNA();
	float raycastLength = 100;
	float raycastAngle = 10;
	float avoidanceLockTime = 40;

	//returns a copy of itself with mutated  values
	AIDNA* asexualReproduction(float mutationRate);

	//returns a new AIDNA object pointer created by mixing and randomizing the values of 2 existing AIDNA objects
	AIDNA* sexualReproduction(float mutationRate, AIDNA* partner);

	//randomizes the variable values in the object.
	void mutate(float mutationRate);

};

