#include "WindArrowObj.h"

WindArrowObj::WindArrowObj(int idNum, std::string renderableName, int xpos, int ypos, int orientation) {
	id = idNum;
	renderable = renderableName;
	x = xpos;
	y = ypos;
	orientation = orientation;
}

WindArrowObj::WindArrowObj(map <string, string> paramsMap, ObjectData* objData) : GameObject(paramsMap, objData) {

}

WindArrowObj::~WindArrowObj() {
}

string WindArrowObj::getObjectType() {
	return "WindArrowObj";
}

void WindArrowObj::earlyUpdate() {

}

void WindArrowObj::midUpdate() {
	
}

void WindArrowObj::lateUpdate() {

}

