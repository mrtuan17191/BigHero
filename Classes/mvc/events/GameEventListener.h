#ifndef __MVCSample__GameEventListener__
#define __MVCSample__GameEventListener__

#include "cocos2d.h"
#include "GameEvent.h"

using namespace cocos2d;

typedef void (Ref::*HandlerFunction)(GameEvent*);
#define handler_function(_function) (HandlerFunction)(&_function)

class GameEventListener: public Ref{
    CC_SYNTHESIZE_RETAIN(Ref*, handlerObject, HandlerObject);
    CC_SYNTHESIZE(HandlerFunction, handlerFunction, HandlerFunction);
    
public:
    GameEventListener();
    virtual ~GameEventListener();
};

#endif
