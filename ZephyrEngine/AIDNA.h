#pragma once
#include <stdlib.h>    
#include <time.h>      
#include <string>	
#include <map>

class AIDNA
{
public:
	AIDNA();
	AIDNA(std::map <std::string, std::string>);
	~AIDNA();
	float raycastLength = 100;
	float raycastAngle = 10;
	float avoidanceLockTime = 40;

	int rating = 0;

	//returns a copy of itself with mutated  values
	AIDNA* asexualReproduction(float mutationRate);

	//returns a new AIDNA object pointer created by mixing and randomizing the values of 2 existing AIDNA objects
	AIDNA* sexualReproduction(float mutationRate, AIDNA* partner);

	//randomizes the variable values in the object.
	void mutate(float mutationRate);

	std::string toString();

};

