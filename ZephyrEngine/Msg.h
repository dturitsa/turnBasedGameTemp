#pragma once
#include "Util.h"

// list all enums
enum MSG_TYPE {
	EMPTY_MESSAGE,

	// Temporary Render System
	RENDER_FRAME_TEST, UPDATE_TEST_OBJ, OBJ_TEST_MSG,

	// Render System
	UPDATE_OBJ_SPRITE,

	// Physics System
	UPDATE_OBJECT_POSITION, //id,renderable,x,y,z,orientation,width,length,physEnabled,type
	DATA_RESPONSE,

	GO_COLLISION,

	// Game System
	GO_REMOVED, GO_ADDED, LOAD_LEVEL, LOAD_MENU, EXIT_GAME, ITEM_SELECTED,
	CHANGE_MAST, CHANGE_RUDDER, REQUEST_DATA, AUDIO_MUTE,

	// IO System 
	CHECK_KEY_PRESSES, TEST_KEY_PRESSED,
	UP_ARROW_PRESSED, DOWN_ARROW_PRESSED, RIGHT_ARROW_PRESSED, LEFT_ARROW_PRESSED, SPACEBAR_PRESSED, KEY_A_PRESSED, KEY_D_PRESSED
};

class Msg {
public:
	Msg(MSG_TYPE t, std::string d);
	~Msg();

	MSG_TYPE type;
	std::string data;

};

