#ifndef GALLERY_SCENE_H
#define GALLERY_SCENE_H

#include "AbstractScene.h"

class GalleryScene : public AbstractScene {
protected:
    UGLayer* backgroundLayer;
    UGLayer* mainLayer;
    Label* skillNameLabel;
    Label* characterLabel;
    AnimatedObject* currentCharacter;
    
    int currentAnimationIndex;
    int currentCharacterIndex;
    bool requestRefresh;
    bool requestRefreshAnimation;
    __Array* totalCharacters;
    __Array* totalAnimations;
private:
    EventListenerTouchAllAtOnce* touchListener;
    EventListenerKeyboard* listener;
    
    void initialize();
    void createLayers();
    void createLayersContent();
    
    virtual void onEnter();
    virtual void onExit();
    
    void addFacesToLayer(Obj* object, Layer* layer, int index);
    void removeFaces(Obj* object);
    
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, Event* event);
    void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event);
    
    void onNextCharacterClick(Ref* sender);
    void onPreviousCharacterClick(Ref* sender);
    void onNextAnimationClick(Ref* sender);
    void onPreviousAnimationClick(Ref* sender);
public:
    GalleryScene();
    ~GalleryScene();
    void update(float dt);
};

#endif
