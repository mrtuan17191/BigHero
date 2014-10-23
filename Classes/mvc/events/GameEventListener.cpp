#include "GameEventListener.h"

GameEventListener::GameEventListener(){
    this->autorelease();
    this->handlerObject = NULL;
    this->handlerFunction = NULL;
}

GameEventListener::~GameEventListener(){
    CC_SAFE_RELEASE(handlerObject);
}