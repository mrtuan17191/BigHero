#include "Controller.h"
#include "../view/View.h"
#include "../handler/GameHandler.h"
#include "../events/GameEvent.h"
#include "../events/GameEventListener.h"
#include "../../utils/Resources.h"

Controller::Controller(){
    this->autorelease();
    handlerList = new Vector<GameHandler*>();
}

Controller::~Controller(){
    delete handlerList;
}

void Controller::start(){
    auto event = new GameEvent();
    event->setEventCode(EVT_START_GAME);
    
    processEvent(event);
}

void Controller::stop(){
    auto event = new GameEvent();
    event->setEventCode(EVT_STOP_GAME);
    
    processEvent(event);
}

void Controller::listen(View *view){
    auto listener = new GameEventListener();
    listener->setHandlerObject(this);
    listener->setHandlerFunction(handler_function(Controller::processEvent));    
    view->addEventListener(listener);
}

void Controller::addHandler(GameHandler* handler){
    if(!getHandlerList()->contains(handler)){
        handler->setController(this);
        getHandlerList()->pushBack(handler);
		handler->setGameModel(gameModel);
    }
}

void Controller::removeHandler(GameHandler *handler){
    if(getHandlerList()->contains(handler)){
        getHandlerList()->eraseObject(handler);
    }
}

void Controller::processEvent(GameEvent* event){
    for (unsigned int i = 0; i < this->getHandlerList()->size(); i++) {
        auto handler = getHandlerList()->at(i);
        handler->processEvent(event);
    }
}