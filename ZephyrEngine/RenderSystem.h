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

	const GLint XSTART = 200, YSTART = 150, WIDTH = 800, HEIGHT = 600; //Constants for drawing the window
	int timeFrame = 10;
private:
	struct Image {
		GLuint texture;
		int width;
		int height;
	};
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
	SDL_Window *window;
	GLuint VBO, VAO, TBO;
	std::mutex mtx;

	void renderAllItems();
	GLchar* getShader(string fileName);
	void Draw(int x, int y, GLuint texture);
	GLuint getTexture(string path);

	std::vector<std::string*> gameObjectsToRender;
	void addObjectToRenderList(Msg* m);
	void removeObjectFromRenderList(Msg* m);
	void updateObjPosition(Msg* m);
};