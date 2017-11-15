#include "RenderSystem.h"

//Vertex Shader
const GLchar *vertexShaderSource = "#version 330 core\n"
"layout ( location = 0 ) in vec3 position;\n"
"layout ( location = 1 ) in vec2 texCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 transform[3];\n" //transform[0] = rotate, transform[1] = scale, transform[2] = translate
"void main()\n"
"{\n"
"gl_Position = transform[2] * transform[1] * transform[0] * vec4( position.x, position.y, position.z, 1.0 );\n"
"TexCoord = vec2( texCoord.x, 1.0f - texCoord.y);\n"
"}";

//Fragment Shader
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture1;\n"
"void main()\n"
"{\n"
"color = texture(ourTexture1, TexCoord);\n"
"}";


RenderSystem::RenderSystem(MessageBus* mbus) : System(mbus) {
	//Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	aspectRatio = (GLfloat)(WIDTH) / (GLfloat)(HEIGHT);
	window = SDL_CreateWindow("Okeanos - Made with Zephyr", RenderSystem::XSTART, RenderSystem::YSTART, RenderSystem::WIDTH, RenderSystem::HEIGHT, SDL_WINDOW_OPENGL);

	SDL_GL_SwapWindow(window);
}


RenderSystem::~RenderSystem() {
}

void RenderSystem::init() {

	//Setup window and context
	context = SDL_GL_CreateContext(window);
	//Setup SDL and GL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to init GLEW" << std::endl;
		return;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	//Make transparent background
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glewExperimental = GL_TRUE;
	glewInit();
	//Set up vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Set up fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		OutputDebugString("DIDNT WORK");
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Set up program
	shaderProgram = glCreateProgram();
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

	//Set up VBOs, VAOs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &TBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Set up texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoord), TexCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glClearColor(0.0f, 0.467f, 0.745f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::draw(string ID, string sprite, float x, float y, float z, float orientation, float width, float height) {
	//Bind transform to vertex shader
	//Create a transform matrix and bind it to shader
	float radRot = orientation * 3.1415927 / 180.0;

	GLfloat* temp = new GLfloat[80]{
		

		cosf(radRot), -sinf(radRot), 0, 0, //Rotate 
		sinf(radRot), cosf(radRot), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,

		getScaleX(width), 0, 0, 0, //Scale
		0, getScaleY(height), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,

		1, 0, 0, 0, //Translate
		0, 1, 0, 0,
		0, 0, 1, 0,
		transX(x), transY(y), 0, 1 //should add z posiiton at tome point

	};
	if (ID == "windmarker") {
		temp = new GLfloat[80]{


			cosf(radRot), -sinf(radRot), 0, 0, //Rotate 
			sinf(radRot), cosf(radRot), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,

			getScaleX(width), 0, 0, 0, //Scale
			0, getScaleY(height), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1,

			1, 0, 0, 0, //Translate
			0, 1, 0, 0,
			0, 0, 1, 0,
			(x / MAX_X), (y / MAX_Y), 0, 1 //should add z posiiton at tome point

		};
	}
	GLint ourTransform = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(ourTransform, 3, GL_FALSE, temp);
	
	glMatrixMode(ourTransform);
	glPushMatrix();
	//glTranslatef(xcenter, ycenter, zcenter); // move back to focus of gluLookAt
	glRotatef(45,0,0,0); //  rotate around center
	//glTranslatef(-xcenter, -ycenter, -zcenter); //move object to center
	//DrawObject();
	//glPopMatrix();

	//Bind texture to fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures.find(sprite)->second);
	GLint ourTextureLocation = glGetUniformLocation(shaderProgram, "ourTexture1");
	glUniform1i(ourTextureLocation, 0);

	//Use shader
	glUseProgram(shaderProgram);

	//Draw
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
	glBindVertexArray(0);

	delete(temp);
}

float RenderSystem::transX(float x) {
	x += cameraX;
	x *= 1 / MAX_X;
	return x;
}

float RenderSystem::transY(float y) {
	y += cameraY;
	y *= 1 / MAX_Y;
	return y;
}
//Scale X based on world scale
float RenderSystem::getScaleX(float x) {
	//Scale sprite down to 1/GAMEWIDTH and apply scale x Default to 1
	float scale = (1 / (GAMEWIDTH / 2.0f)) * ((x == 0) ? 1.0f : x);
	return scale;
}

//Scale Y based on world scale
float RenderSystem::getScaleY(float y) {
	//Scale sprite down to 1/GAMEHEIGHT and apply scale y Default 1
	float scale = (1 / (GAMEHEIGHT / 2.0f)) * ((y == 0) ? 1.0f : y);
	return scale;
}
void RenderSystem::renderAllItems() {
	//RENDER MENU SCREEN
	for (string* s : gameObjectsToRender) {
		//std::vector<std::string> data = split(s, ',');
		//OutputDebugString(s->c_str());
		//OutputDebugString("\n");
		renderObject(*s);
	}
}

void RenderSystem::renderObject(string object) {
	//object format: ID,png,x,y,z,orientation
	string ID, sprite;
	float x, y, z, orientation, w, h;

	//Split object
	vector<string> objectData = split(object, ',');

	//Parse object data
	ID = objectData[0];
	sprite = objectData[1];
	x = atof(objectData[2].c_str());
	y = atof(objectData[3].c_str());
	z = atof(objectData[4].c_str());
	orientation = atof(objectData[5].c_str());
	w = atof(objectData[6].c_str());
	h = atof(objectData[7].c_str());

	/*ID = object->ID;
	sprite = object->sprite;
	x = object->x;
	y = object->y;
	z = object->z;
	orientation = object->orientation;
	w = object->w;
	h = object->h;*/

	//Load texture into memory if it is not already 
	//(probably not the right way to do it)
	map<string, GLuint>::iterator it = textures.find(sprite);
	if (it == textures.end()) {
		textures.insert(pair<string, GLuint>(sprite, getTexture(sprite)));
	}
	//Draw object
	draw(ID, sprite, x, y, z, orientation,w,h);
}
GLuint RenderSystem::getTexture(string path) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_Surface *temp = IMG_Load(path.c_str());

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp->w, temp->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	const char* sdlError = SDL_GetError();
	GLenum glError = glGetError();
	if (strlen(sdlError) > 0) {
		std::cout << sdlError << std::endl;
	}
	if (glError != GL_NO_ERROR) {
		std::cout << glError << std::endl;
	}

	SDL_FreeSurface(temp);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

void RenderSystem::startSystemLoop() {
	init();
	running = true;
	clock_t thisTime = clock();
	//clock_t lastTime = thisTime;
	SDL_Event windowEvent;

	int renderCount = 0;

	int currentGameTime = 0;
	while (running) {
		thisTime = clock();
		if (thisTime  < currentGameTime) {
			Sleep(currentGameTime - thisTime);
		}
		currentGameTime += timeFrame;

		handleMsgQ();

		////Display Thread ID for Debugging
		//std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));
		//OutputDebugString("Render Loop on thread: ");
		//OutputDebugString(s.c_str());
		//OutputDebugString("\n");

		//string str = to_string(renderCount);
		//OutputDebugString(str.c_str());
	//OutputDebugString("\n");
	//	renderCount++;

		//lastTime = thisTime;

		//mtx.lock();
		//Clear the screen
		glClearColor(0.0f, 0.467f, 0.745f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Render all objects
		renderCount++;
		string st = to_string(renderCount);
	//	OutputDebugString(st.c_str());
		//OutputDebugString("\n");
		//mtx.lock();
		renderAllItems();
	//	mtx.unlock();
		//Update openGL window
		SDL_GL_SwapWindow(window);
		//mtx.unlock();
		
	}
}

void RenderSystem::stopSystemLoop() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	running = false;

}
void RenderSystem::handleMessage(Msg *msg) {
	std::string s = std::to_string(std::hash<std::thread::id>()(std::this_thread::get_id()));

	////Display Thread ID for Debugging
	//OutputDebugString("Render  Handle Messsage on thread: ");
	//OutputDebugString(s.c_str());
	//OutputDebugString("\n");
	// call the parent first 
	System::handleMessage(msg);

	// personal call 
	switch (msg->type) {
	case UPDATE_OBJECT_POSITION:
		//mtx.lock();
		updateObjPosition(msg);
	//	mtx.unlock();
		break;
	case GO_ADDED:
		// parse data from msg to add obj to list
		//mtx.lock();
		addObjectToRenderList(msg);
		//mtx.unlock();
		break;
	case GO_REMOVED:
	//	mtx.lock();
		removeObjectFromRenderList(msg);
	//	mtx.unlock();
		break;
	case UPDATE_OBJ_SPRITE:
		updateObjSprite(msg);
		break;

	case LEVEL_LOADED:
		levelLoaded(msg);
		break;
	//PANNING CAMERA
	case SPACEBAR_PRESSED:
		if (loadedLevel == 2) {
			cameraToPlayer();
		}
		break;
	case UP_ARROW_PRESSED:
		if (loadedLevel == 2) {
			panUp();
		}
		break;
	case DOWN_ARROW_PRESSED: 
		if (loadedLevel == 2) {
			panDown();
		}
		break;
	case RIGHT_ARROW_PRESSED:
		if (loadedLevel == 2) {
			panRight();
		}
		break;
	case LEFT_ARROW_PRESSED:
		if (loadedLevel == 2) {
			panLeft();
		}
		break;
	default:
		break;
	}
}

void RenderSystem::removeObjectFromRenderList(Msg* m) {
	for (vector<string*>::iterator it = gameObjectsToRender.begin(); it != gameObjectsToRender.end(); it++){
		std::vector<std::string> obj = split(**it, ',');
		// found the obj
		if (obj.front() == m->data) {
			gameObjectsToRender.erase(it);
			return;
		}
	}
	/*
	for (auto s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');
		// found the obj
		if (obj.front() == m->data) {
			// remove the object
			gameObjectsToRender.erase(s);
			return;
		}
	}
	
	gameObjectsToRender.push_back(&m->data);
	*/
	
}

void RenderSystem::addObjectToRenderList(Msg* m) {
	//mtx.lock();
	gameObjectsToRender.push_back(&m->data);
//	mtx.unlock();
}

void RenderSystem::updateObjPosition(Msg* m) {
	std::vector<std::string> dataVector = split(m->data, ',');

	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');

		int position = 0;
		std::ostringstream oss;

		// found the obj
		if (obj.front() == dataVector.front()) {
			// replace this string's information with new information except the renderable
			for (std::string ss : obj) {
				if (position != 1) {
					oss << dataVector.at(position) << ",";
				} else if (position == 1) {
					oss << obj.at(1) << ",";
				}

				position++;
			}
			*s = oss.str();
			return;
		}

	}
}

// id, [dont read], renderable name
void RenderSystem::updateObjSprite(Msg* m) {
	std::vector<std::string> dataVector = split(m->data, ',');

	std::ostringstream oss;

	for (std::string* s : gameObjectsToRender) {
		std::vector<std::string> obj = split(*s, ',');

		// found the obj
		if (obj.front() == dataVector.front()) {

			int position = 0; // renderable is position 1; comes right after the id
			
			// copy everything except the renderable
			for (std::string ss : obj) {
				if (position != 1) {
					oss << ss << ",";
				} else {
					// put renderable in
					oss << dataVector.at(2) << ",";
				}

				position++;				
			}

			// replace with data
			*s = oss.str();
			return;
		}

	}
}

void RenderSystem::panLeft() {
	if (cameraX + CAMERAPAN_X <= maxCameraX) {
		cameraX += CAMERAPAN_X;
	}
}

void RenderSystem::panRight() {
	if (cameraX - CAMERAPAN_X >= minCameraX) {
		cameraX -= CAMERAPAN_X;
	}
}

void RenderSystem::panUp() {
	if (cameraY - CAMERAPAN_Y >= minCameraY) {
		cameraY -= CAMERAPAN_Y;
	}
}

void RenderSystem::panDown() {
	if (cameraY + CAMERAPAN_Y <= maxCameraY) {
		cameraY += CAMERAPAN_Y;
	}
}

void RenderSystem::cameraToPlayer() {
	for (string* s : gameObjectsToRender) {
		vector<string> obj = split(*s, ',');

		if (obj.at(0) == "playerShip") {
			cameraX = -atof(obj.at(2).c_str());
			cameraY = -atof(obj.at(3).c_str());
			break;
		}
	}
}

void RenderSystem::levelLoaded(Msg* m) {
	loadedLevel = atoi(m->data.c_str());
}