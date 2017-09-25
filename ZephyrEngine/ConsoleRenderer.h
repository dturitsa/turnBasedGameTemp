#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <Windows.h>

/*
an object to be rendered
This should eventually contain all of the gameobject info required to render it including the sprite used and object position.

The renderer and gameobject components will need access to this... something something treadsafety
*/
class RenderObj {

private:

	char displayChar; //the char to be displayed
	int xPos; //the xposition on the screen
	int yPos; //the y position

public:
	//main constructor adds a position and character to display
	RenderObj(char c, int x, int y);

	//getters and setters
	void setX(int x);
	void setY(int y);
	void setDisplayChar(char c);
	int getX();
	int getY();
	char getDisplayChar();
};


//responsible for rendering the RenderObj's
//there should probably be only one instance of this guy. Maybe a second one for the minimap

class Renderer {
	std::list<RenderObj*> renderObjList; //list of objects to render
	std::list<RenderObj*>::iterator it;
	char screenViewArr[20][20]; //array representing what is to appear on the screen
public:

	//constructor for the Renderer
	Renderer();


	//add a renderObj to the list of object to render
	void addRenderObj(RenderObj* r);
	

	//removes a renderObj from list of objects to render. Do this to avoid null pointers after destroying an object

	void removeRenderObj(RenderObj* r);

	//renders a frame by outputting the contents of the screenView array
	void renderFrame();


private:
	//updates what objects are displayed on the screen and in what position
	void updateCameraView();

};