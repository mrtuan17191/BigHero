#ifndef ABSTRACT_SCENE_H
#define ABSTRACT_SCENE_H

#include "cocos2d.h"
#include "../../mvc/include/mvc.h"
#include "../../models/include/models.h"
#include "../../events/include/events.h"
#include "../../AppMacros.h"
#include "../override/UGLayer.h"
#include "../override/UGLayerColor.h"
#include "../override/UGMenu.h"

USING_NS_CC;

class AbstractScene: public Scene, public View {
protected:
    __Dictionary* nodes;
    
    virtual void onEnter();
    virtual void onExit();
public:
	AbstractScene(void);
	virtual ~AbstractScene(void);
    
    void checkFlash(MenuItemSprite* button);
    void handleFlashButton(Ref* button);
    void flash(Sprite* sprite);
    void flashCallback(Ref* sender);
    void flashOn(Sprite* sprite);
    void flashOff(Sprite* sprite);
};
#endif

