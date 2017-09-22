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
class RenderObj{

	private:

		char displayChar; //the char to be displayed
		int xPos; //the xposition on the screen
		int yPos; //the y position

	public:
		//main constructor adds a position and character to display
		RenderObj(char c, int x, int y){
			displayChar = c;
			xPos = x;
			yPos = y;
		}

		//getters and setters
		void setX(int x){
			xPos = x;
		}

		void setY(int y){
			yPos = y;
		}

		void setDisplayChar(char c){
			displayChar = c;
		}

		int getX(){
			return xPos;
		}

		int getY(){
			return yPos;
		}

		char getDisplayChar(){
			return displayChar;
		}
};

/*responsible for rendering the RenderObj's 
there should probably be only one instance of this guy. Maybe a second one for the minimap
*/
class Renderer{
	std::list<RenderObj*> renderObjList; //list of objects to render
	std::list<RenderObj*>::iterator it; 
	char screenViewArr[20][20]; //array representing what is to appear on the screen
	public:

		//constructor for the Renderer
		Renderer(){
			//makes some space on the screen
			for(int k = 0; k<20; k++)
				std::cout << "\n";
		}

		/*
		add a renderObj to the list of object to render
		r - a pointer to the renderObj
		*/
		void addRenderObj(RenderObj* r){
			renderObjList.push_back(r); 
		}

		/* 
		removes a renderObj from list of objects to render
		do this to avoid null pointers after destroying an object
		*/
		void removeRenderObj(RenderObj* r){
			renderObjList.remove(r); 
		}

		//renders a frame by outputting the contents of the screenView array
		void renderFrame(){
			updateCameraView();

			for(int k = 0; k<20; k++)
				std::cout << "\x1b[A";

			for(int i = 0; i<20; i++){
				for(int j = 0; j < 20; j++){
					std::cout << screenViewArr[i][j] << ' ';
				}
					
				std::cout << '\n';
			}

		}

	private:
		//updates what objects are displayed on the screen and in what position
		void updateCameraView(){
			int x, y;
			char c;

			//clear the screenViewArr
			for(int i = 0; i<20; i++)
				for(int j = 0; j < 20; j++)
					screenViewArr[i][j] = '_';

			//set add the renderObj's ot the proper place in the screen array (if they are in view)
			for (it=renderObjList.begin(); it!=renderObjList.end(); ++it){
				x = (*it)->getX();
				y = (*it)->getY();
				c = (*it)->getDisplayChar();
				if(x < 20 && x >= 0 && y < 20 && y >= 0)
	    			screenViewArr[x][y] = c;
			}
		}
};

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