#ifndef __MVCSample__Handler__
#define __MVCSample__Handler__

#include "cocos2d.h"

using namespace cocos2d;

class Controller;
class GameEvent;
class GameModel;

class GameHandler: public Ref {
    CC_SYNTHESIZE(Controller*, controller, Controller);	
	CC_SYNTHESIZE(GameModel*, gameModel, GameModel);
public:
    GameHandler();
    virtual ~GameHandler();
    virtual void processEvent(GameEvent* event);
};

#endif
