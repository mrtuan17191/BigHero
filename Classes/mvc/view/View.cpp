#include "View.h"
#include "../events/GameEvent.h"
#include "../events/GameEventListener.h"
#include "../model/Model.h"

View::View() {
    this->eventListeners = new Vector<GameEventListener*>();
}

View::~View() {
    delete eventListeners;
}

void View::addEventListener(GameEventListener* eventListener) {
    if(!getEventListeners()->contains(eventListener)) {
        getEventListeners()->pushBack(eventListener);
    }
}

void View::removeEventListener(GameEventListener* eventListener) {
    if(getEventListeners()->contains(eventListener)){
        getEventListeners()->eraseObject(eventListener);
    }
}

void View::fireEvent(GameEvent *event){
    for (unsigned int i = 0; i< getEventListeners()->size(); i++) {
        auto listener = getEventListeners()->at(i);
        
        auto handlerObject = listener->getHandlerObject();
        auto handlerFunction = listener->getHandlerFunction();
        
        (handlerObject->*handlerFunction)(event);
    }
}