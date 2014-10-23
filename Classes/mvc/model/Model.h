#ifndef __MVCSample__Model__
#define __MVCSample__Model__

#include "cocos2d.h"

using namespace cocos2d;

class GameEvent;
class GameEventListener;

class Model: public Ref{
    CC_SYNTHESIZE(Vector<GameEventListener*>*, eventListeners, EventListeners);
public:
    Model();
    virtual ~Model();
    void addEventListener(GameEventListener* eventListener);
    void removeEventListener(GameEventListener* eventListener);
    void fireEvent(GameEvent* event);
};


#endif
