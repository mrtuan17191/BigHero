#ifndef ABSTRACT_LAYER_H
#define ABSTRACT_LAYER_H

#include "../../utils/include/utils.h"
#include "../../models/include/models.h"
#include "../../AppMacros.h"
#include "../override/UGLayer.h"
#include "../override/UGLayerColor.h"
#include "../override/UGMenu.h"

USING_NS_CC;

class AbstractLayer : public Layer, public View {
protected:
    __Dictionary* nodes;
    
    virtual void onEnter();
    virtual void onExit();
public:
    AbstractLayer();
    ~AbstractLayer();
    
    void checkFlash(MenuItemSprite* button);
    void handleFlashButton(Ref* button);
    void flash(Sprite* sprite);
    void flashCallback(Ref* sender);
    void flashOn(Sprite* sprite);
    void flashOff(Sprite* sprite);
};

#endif