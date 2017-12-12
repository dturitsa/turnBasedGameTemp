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

	
	int rating = 0; //how well this combination of "genes" performs
	int inUse = 0; //the number of active AIobjects are currently using this configuration

	//The "genes" that can be adjusted by evolution. these are used in the AIObject class
	float raycastLength = 100;
	float raycastAngle = 10;
	float avoidanceLockTime = 40;
	float seekDistance = 90;
	float engageDistance = 80;

	//returns a copy of itself with mutated  values
	AIDNA* asexualReproduction(float mutationRate);

	//returns a new AIDNA object pointer created by mixing and randomizing the values of 2 existing AIDNA objects
	AIDNA* sexualReproduction(float mutationRate, AIDNA* partner);

	//randomizes the genes values in this AIDNA object instance.
	void mutate(float mutationRate);

	//string representation of the object values (for saving to text file)
	std::string toString();

};

