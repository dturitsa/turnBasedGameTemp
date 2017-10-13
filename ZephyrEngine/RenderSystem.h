#pragma once
#include "System.h"
#include "GameObject.h"
#include <mutex>

#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <freeglut.h>

class RenderSystem : public System {
public:
	RenderSystem(MessageBus* mbus);
	~RenderSystem();

	void handleMessage(Msg * msg);
	void startSystemLoop();
	void stopSystemLoop();
	void init();

	const GLint XSTART = 200, YSTART = 150, WIDTH = 800, HEIGHT = 600; //Constants for drawing the window
	int timeFrame = 10;
private:
	bool running;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	SDL_Window *window;
	SDL_GLContext context;
	GLuint VBO, VAO, TBO;
	std::mutex mtx;
	//Vertecies for a quad
	GLfloat vertices[12] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, 0.5f, 0.0f,  // top left
		0.5f, 0.5f, 0.0f,  // top right
	};

	//Texture coordinates for quad
	GLfloat TexCoord[10] = {
		0, 0,
		1, 0,
		0, 1,
		1, 1
	};

	//Indices to reuse vertecies
	GLubyte indices[10] = { 0,1,2, // first triangle (bottom left - bottom right - top left)
		1,2,3 }; // second triangle (bottom right - top left - top right)
	
	void renderAllItems();
	void draw(string ID, string path, float x, float y, float z, float rotation);
	GLuint getTexture(string path);
	void renderObject(string object);

	std::vector<std::string*> gameObjectsToRender;
	void addObjectToRenderList(Msg* m);
	void removeObjectFromRenderList(Msg* m);
	void updateObjPosition(Msg* m);

};