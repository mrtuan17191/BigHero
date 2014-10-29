#include "Elevator.h"

Elevator::Elevator(int type, Point pos) {
    this->type = type;
    this->pos = pos;
}

Elevator::~Elevator() {
}

void Elevator::initFaces(std::string key, __Array* faceStates) {
    AnimatedObject::initFaces(key, faceStates);
    Face* charFace = this->getFace((char*) FACE_CHAR);
    
    RectBody* body = new RectBody(charFace->getContentSize().width, charFace->getContentSize().height);
    CC_SAFE_RETAIN(body);
    this->body = body;
    this->body->setScale(CONF_SCALE_GAME);
    
    charFace->setState(charFace->getState(STATE_DOOR1_IDLE));
}