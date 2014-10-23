#ifndef UGLAYER_H
#define UGLAYER_H

#include "cocos2d.h"
USING_NS_CC;

class UGLayer : public Layer {
private:
    virtual void onEnter();
    virtual void onExit();
public:
    CREATE_FUNC(UGLayer);
};

#endif