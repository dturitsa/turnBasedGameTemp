#include "ConsoleRenderer.h"


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