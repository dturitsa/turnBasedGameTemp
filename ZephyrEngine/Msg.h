#pragma once
#include "Util.h"

// list all enums
enum MSG_TYPE {
	// Temporary Render System
	RENDER_FRAME_TEST, UPDATE_TEST_OBJ, OBJ_TEST_MSG,

	// Render System
	UPDATE_OBJECT_POSITION, NEW_OBJECT_TO_RENDER, REMOVE_OBJECT_FROM_RENDER,

	// Physics System

	// Game System

	// IO System 
	CHECK_KEY_PRESSES, TEST_KEY_PRESSED
};

class Msg {
public:
	Msg(MSG_TYPE t, std::string d);
	~Msg();

	MSG_TYPE type;
	std::string data;

};

