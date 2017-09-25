#include "ConsoleRenderer.h"


/*
an object to be rendered
This should eventually contain all of the gameobject info required to render it including the sprite used and object position.

The renderer and gameobject components will need access to this... something something treadsafety
*/






//main constructor adds a position and character to display
RenderObj::RenderObj(char c, int x, int y) {
		displayChar = c;
		xPos = x;
		yPos = y;
	}

	//getters and setters
	void RenderObj::setX(int x) {
		xPos = x;
		if (xPos > 19) {
			xPos = 19;
		}

		if (xPos < 0) {
			xPos = 0;
		}
	}

	void RenderObj::setY(int y) {
		yPos = y;

		if (yPos > 19) {
			yPos = 19;
		}

		if (yPos < 0) {
			yPos = 0;
		}
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
		//makes some space on the screen
		for (int k = 0; k < 20; k++) {
			std::cout << "\n";
		}
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


// UNUSED CODE - FOR REFERENCE ONLY 
/*



//Main handles the stuff that should be in the gameloop and gameobject components
int main()
{
	//creates a new renderer object to handle the console display
	Renderer* rendererPointer = new Renderer();

	//creates some render objects
	RenderObj* b = new RenderObj('b', 3, 1);
	rendererPointer->addRenderObj(b);

	RenderObj* c = new RenderObj('c', 1, 4);
	rendererPointer->addRenderObj(c);

	RenderObj* d = new RenderObj('d', 7, 7);
	rendererPointer->addRenderObj(d);

	RenderObj* a = new RenderObj('^', 1, 1);
	rendererPointer->addRenderObj(a);

	//the temporary game loop
	while(1){
		//tell the renderer to render the next frame and wait for a bit
		rendererPointer->renderFrame();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//update the renderObj between frames and the renderer will update them as well
		if(GetKeyState('W') & 0x8000){
			int temp = a->getX();
			temp--;
			a->setX(temp);
		}

		if(GetKeyState('S') & 0x8000){
			int temp = a->getX();
			temp++;
			a->setX(temp);
		}

		if(GetKeyState('A') & 0x8000){
			int temp = a->getY();
			temp--;
			a->setY(temp);
		}

		if(GetKeyState('D') & 0x8000){
			int temp = a->getY();
			temp++;
			a->setY(temp);
		}		
	}
	return 0;  
}

*/