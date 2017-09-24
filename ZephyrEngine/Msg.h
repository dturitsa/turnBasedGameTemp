#pragma once
#include <string>

// list all enums
enum MSG_TYPE {
	// Temporary Render System
	RENDER_FRAME_TEST, UPDATE_TEST_OBJ, 
	
	// IO System 
	CHECK_KEY_PRESSES
};

class Msg {
public:
	Msg(MSG_TYPE t, std::string d);
	~Msg();

	MSG_TYPE type;
	std::string data;

};

