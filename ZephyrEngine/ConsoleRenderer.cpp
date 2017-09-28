#include "ConsoleRenderer.h"

//main constructor adds a position and character to display
RenderObj::RenderObj(char c, int x, int y) {
		displayChar = c;
		xPos = x;
		yPos = y;
	}

	//getters and setters
	void RenderObj::setX(int x) {
		xPos = x;
		/*
		if (xPos > 19) {
			xPos = 19;
		}

		if (xPos < 0) {
			xPos = 0;
		}
		*/
	}

	void RenderObj::setY(int y) {
		yPos = y;
		/*
		if (yPos > 19) {
			yPos = 19;
		}

		if (yPos < 0) {
			yPos = 0;
		}
		*/
	}

	void RenderObj::setDisplayChar(char c) {
		displayChar = c;
	}

	int RenderObj::getX() {
		return xPos;
	}

	int RenderObj::getY() {
		return yPos;
	}

	char RenderObj::getDisplayChar() {
		return displayChar;
	}




//constructor for the Renderer
Renderer::Renderer() {

	}

	/*
	add a renderObj to the list of object to render
	r - a pointer to the renderObj
	*/
void Renderer::addRenderObj(RenderObj* r) {
		renderObjList.push_back(r);
	}

	/*
	removes a renderObj from list of objects to render
	do this to avoid null pointers after destroying an object
	*/
void Renderer::removeRenderObj(RenderObj* r) {
		renderObjList.remove(r);
	}

	//renders a frame by outputting the contents of the screenView array
void Renderer::renderFrame() {
		updateCameraView();



		// NOTE: THIS CHARACTER DOESN'T WORK IN WINDOWS
		/*
		for (int k = 0; k<20; k++)
		std::cout << "\x1b[A";
		*/

		// This is the least hacky workaround I could find
		system("cls");

	
		for (int i = 0; i< 20; i++) {
			for (int j = 0; j < 20; j++) {
				std::cout << screenViewArr[i][j] << ' ';
			}
			std::cout << '\n';
		}


	}

	//updates what objects are displayed on the screen and in what position
void Renderer::updateCameraView() {
	int x, y;
	char c;

	//clear the screenViewArr
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			screenViewArr[i][j] = '_';
		}
	}

	//set add the renderObj's ot the proper place in the screen array (if they are in view)
	for (it = renderObjList.begin(); it != renderObjList.end(); ++it) {
		x = (*it)->getX();
		y = (*it)->getY();
		c = (*it)->getDisplayChar();
		if (x < 20 && x >= 0 && y < 20 && y >= 0)
			screenViewArr[x][y] = c;
	}
}