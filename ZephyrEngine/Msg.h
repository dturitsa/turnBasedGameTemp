#pragma once
#include <string>

class Msg {
public:
	Msg();
	~Msg();

	// name our int types, eg: 001 = RENDER_SCENE
	// 002 = PLAY_SOUND 
	// or whatever we decide later.
	int type;

};

