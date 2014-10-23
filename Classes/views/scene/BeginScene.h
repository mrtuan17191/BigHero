#ifndef BEGIN_SCENE_H
#define BEGIN_SCENE_H

#include "AbstractScene.h"
#include "../layer/OptionLayer.h"
#include "../layer/ConfirmLayer.h"

class BeginScene : public AbstractScene {
protected:
    UGLayer* backgroundLayer;
    UGLayer* mainLayer;
    UGMenu* mainMenu;
    OptionLayer* optionLayer;
    ConfirmLayer* confirmLayer;
    
    EventListenerKeyboard* listener;
private:
    void initialize();
    void createLayers();
    void createLayersContent();
    
    virtual void onEnter();
    virtual void onExit();
    
    void processEvent(GameEvent* gameEvent);
    
    void onStartGameClick(Ref* sender);
    void onOptionsClick(Ref* sender);
    
    void onShareFbClick(Ref* sender);
    void onShareTwClick(Ref* sender);
    void onRateClick(Ref* sender);
    void onRemoveAdsClick(Ref* sender);
    void onRestoreAdsClick(Ref* sender);

    void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event);
public:
    BeginScene();
    ~BeginScene();
    
    void update(float dt);
};

#endif
