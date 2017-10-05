#pragma once
#include "Msg.h"
#include <map>
#include <iostream>
using namespace std;

class GameObject {
public:
	std::string id;
	int x;
	int y;
	int width;
	int length;
	int orientation; // in degrees, 0 pointing up on the screen, clockwise rotation = positive
	int speed; // can be zero.
	
	// in degrees, same as orientation; difference is that orientation is used for rendering, direction is the direction of movement
	// this CAN BE DIFFERENT from orientation; eg: cannon ball orientated one direction, but affecteed by the wind will curve.
	int direction; 

	std::string renderable; // name of a file/sprite? probably needs to be changed

	GameObject();
	~GameObject();
	GameObject::GameObject(map <string, string> paramsMap);
	virtual std::string getObjectType();
	virtual std::string toString();
	void earlyUpdate();
	void midUpdate();
	void lateUpdate();
protected:

private:

};