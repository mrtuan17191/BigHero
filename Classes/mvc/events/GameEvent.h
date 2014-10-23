#ifndef __MVCSample__GameEvent__
#define __MVCSample__GameEvent__

#include "cocos2d.h"
using namespace cocos2d;

class GameEvent: public Ref{
    CC_SYNTHESIZE(int, eventCode, EventCode);
public:
    GameEvent();
    virtual ~GameEvent();
};

#endif
