#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "AbstractScene.h"
#include "../layer/ControllerLayer.h"
#include "../layer/PadLayer.h"
#include "../layer/OptionLayer.h"

class GameScene : public AbstractScene {
protected:
    UGLayer* backgroundLayer;
    UGLayer* mainLayer;
    UGMenu* mainMenu;
    ControllerLayer* controllerLayer;
    PadLayer* padLayer;
    UGLayer* guiLayer;
    UGLayer* pauseLayer;
    
    StatusBar* hpBar;
    StatusBar* angryBar;
    StatusBar* weaponBar;
    StatusBar* bossBar;
    Label* gameTimeLabel;
    Label* scoreLabel;
private:
    GameEventListener* gameEventListener;
    EventListenerTouchAllAtOnce* touchListener;
    EventListenerKeyboard* listener;
    
    void initialize();
    void createLayers();
    void createLayersContent();
    
    virtual void onEnter();
    virtual void onExit();
    
    void processEvent(GameEvent* gameEvent);
    void addFacesToLayer(Obj* object, Layer* layer, int index);
    void removeFaces(Obj* object);
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);
    void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event);
    
    void animationCallback(Ref* sender);
    void fadeInCallback(Ref* sender);
    void fadeOutCallback(Ref* sender);
    void almostDieCallback(Ref* sender);
    void selfDestroyCallback(Ref* sender);
    
    void onPauseBoardMoveUpDone();
    void onPauseBoardMoveDownDone();
    
    void onPauseButtonClick(Ref* sender);
    void onResumeButtonClick(Ref* sender);
    void onOptionsButtonClick(Ref* sender);
    void onQuitButtonClick(Ref* sender);
public:
    GameScene();
    ~GameScene();
    void update(float dt);
};

#endif
