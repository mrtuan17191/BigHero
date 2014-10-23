
#include "Model.h"
#include "../events/GameEvent.h"
#include "../events/GameEventListener.h"

Model::Model(){
    this->autorelease();
    this->eventListeners = new Vector<GameEventListener*>();
}

Model::~Model(){
    delete eventListeners;
}

void Model::addEventListener(GameEventListener* eventListener){
    if(!getEventListeners()->contains(eventListener)){
        getEventListeners()->pushBack(eventListener);
    }
}

void Model::removeEventListener(GameEventListener* eventListener){
    if(getEventListeners()->contains(eventListener)){
        getEventListeners()->eraseObject(eventListener);
    }
}


void Model::fireEvent(GameEvent *event){
    for (unsigned int i = 0; i < getEventListeners()->size(); i++) {
        auto listener = getEventListeners()->at(i);
        
        auto handlerObject = listener->getHandlerObject();
        auto handlerFunction = listener->getHandlerFunction();
        
        (handlerObject->*handlerFunction)(event);
    }
}

