#include "RenderSystem.h"

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4( position.x, position.y, position.z, 1.0 );\n"
"}";

const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
"}";

RenderSystem::RenderSystem(MessageBus* mbus) : System (mbus) {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	//Setup SDL and GL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//Setup window and context
	SDL_Window *window = SDL_CreateWindow("SDL OpenGL", XSTART, YSTART, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
	std::cout << "Failed to init GLEW" << std::endl;
		return;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	SDL_Event windowEvent;

	while (true) {
		if (SDL_PollEvent(&windowEvent)) {
			if (SDL_QUIT == windowEvent.type) {
				break;
			}
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//draw OpenGL

		SDL_GL_SwapWindow(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	//Window was closed
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


RenderSystem::~RenderSystem() {
}


void RenderSystem::renderAllItems() {
	// temporary system: just prints out the data for each item
	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> data = split(*s, ',');
		std::cout << *s << "\n"; 
	}
	
}

void RenderSystem::startSystemLoop() {
	clock_t thisTime = clock();
	clock_t lastTime = thisTime;
	
	while (true) {
		thisTime = clock();
		if ((thisTime - lastTime) > timeFrame) {
			lastTime = thisTime;
			mtx.lock();
			renderAllItems();
			mtx.unlock();
		}
	}
}

void RenderSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case UPDATE_OBJECT_POSITION:
		mtx.lock();
		updateObjPosition(msg);
		mtx.unlock();
		break;
	case GO_ADDED:
		// parse data from msg to add obj to list
		mtx.lock();
		addObjectToRenderList(msg);
		mtx.unlock();
		break;
	case GO_REMOVED:
		mtx.lock();
		removeObjectFromRenderList(msg);
		mtx.unlock();
		break;
	default:
		break;
	}
}

void RenderSystem::removeObjectFromRenderList(Msg* m) {
	for (auto s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		// found the obj
		if (obj.front() == m->data) {
			// remove the object

			return;
		}
	}
	gameObjectsToRender.push_back(&m->data);
}

void RenderSystem::addObjectToRenderList(Msg* m) {
	gameObjectsToRender.push_back(&m->data);
}

void RenderSystem::updateObjPosition(Msg* m) {
	std::vector<std::string> data = split(m->data, ',');
	
	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		
		// found the obj
		if (obj.front() == data.front()) {
			// replace this string's information with new information
			*s = m->data;
			return;
		}

	}
}