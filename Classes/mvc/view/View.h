#ifndef __MVCSample__View__
#define __MVCSample__View__

#include "cocos2d.h"
#include "../../utils/include/utils.h"

using namespace cocos2d;

class GameEvent;
class GameEventListener;
class Model;

class View {
    UG_PROPERTY_FULL(Vector<GameEventListener*>*, eventListeners, EventListeners);
public:
    View();
    virtual ~View();
    void addEventListener(GameEventListener* eventListener);
    void removeEventListener(GameEventListener* eventListener);
    void fireEvent(GameEvent* event);
};

#endif
