#ifndef UGLAYER_COLOR_H
#define UGLAYER_COLOR_H

#include "cocos2d.h"
USING_NS_CC;

class UGLayerColor : public LayerColor {
private:
    virtual void onEnter();
    virtual void onExit();
public:
    bool initWithColor(const Color4B& color);
    CREATE_FUNC(UGLayerColor);
};

#endif