#ifndef __MVCSample__Controller__
#define __MVCSample__Controller__

#include "cocos2d.h"

using namespace cocos2d;

class View;
class GameHandler;
class GameEvent;
class GameModel;

class Controller: public Ref{
    CC_SYNTHESIZE(Vector<GameHandler*>*, handlerList, HandlerList);
	CC_SYNTHESIZE(GameModel*, gameModel, GameModel);
public:
    Controller();
	Controller(Size* displayResolutionSize,Point* vectorMove);
    virtual ~Controller();    
    void start();
    void stop();
    void listen(View* view);
    void addHandler(GameHandler* handler);
    void removeHandler(GameHandler* handler);
    void processEvent(GameEvent* event);
};

#endif
