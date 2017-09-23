#pragma once
#include <string>

// list all enums
enum MSG_TYPE {
	RENDER_FRAME_TEST, UPDATE_TEST_OBJ
};

class Msg {
public:
	Msg(MSG_TYPE t, std::string d);
	~Msg();

	MSG_TYPE type;
	std::string data;

};

