#ifndef CHAPTER_SCENE_H
#define CHAPTER_SCENE_H

#include "AbstractScene.h"
#include "../layer/ShareLayer.h"

class ChapterScene : public AbstractScene {
protected:
    UGLayer* backgroundLayer;
    UGLayer* mainLayer;
    EventListenerTouchOneByOne* touchListener;
    EventListenerKeyboard* listener;
    UGLayerColor* chapterLayer;
    ShareLayer* shareLayer;
    Point chapterLayerOffset;
    Size chapterTempSize;

    int currentPage;    
    bool isMoved;
    bool isTouched;
    bool chapterMoving;
    Point firstPoint;
    Point movePoint;
private:
    void initialize();
    void createLayers();
    void createLayersContent();
    
    virtual void onEnter();
    virtual void onExit();
    
    void processEvent(GameEvent* gameEvent);
    void backButtonClick(Ref* sender);
    void onMoveChapterDone();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    void updateChapterLayer(Point point);
    void updatePageAuto();
    void updatePage(int page);
    
    void onKeyPress(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyRelease(EventKeyboard::KeyCode keyCode, Event* event);
public:
    ChapterScene();
    ~ChapterScene();
    
    void update(float dt);
    void showCongratulationMessage();
};

#endif
