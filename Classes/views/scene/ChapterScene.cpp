#include "ChapterScene.h"

ChapterScene::ChapterScene() {
    this->touchListener = NULL;
    this->isTouched = false;
    this->isMoved = false;
    this->chapterMoving = false;
    this->currentPage = 1;
    this->initialize();
}

ChapterScene::~ChapterScene() {
}

void ChapterScene::onEnter() {
    AbstractScene::onEnter();
}

void ChapterScene::onExit() {
    AbstractScene::onExit();
    this->removeAllChildren();
}

void ChapterScene::createLayers() {
    this->backgroundLayer = UGLayer::create();
    this->addChild(this->backgroundLayer);
    
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
    
    this->shareLayer = new ShareLayer(this);
    this->shareLayer->setVisible(false);
    this->addChild(this->shareLayer);
}

void ChapterScene::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    Face* backgroundFace = new Face();
    backgroundFace->initWithFile(ANI_BACKGROUND);
    backgroundFace->setScale(CONF_FAKE_1);
    backgroundFace->setPosition(displayResolutionSize.width / 2, displayResolutionSize.height / 2);
    this->backgroundLayer->addChild(backgroundFace);
    
    Face* chapterTitle = new Face();
    chapterTitle->initWithSpriteFrameName(ANI_CHAPTER_TITLE);
    chapterTitle->setPosition(displayResolutionSize.width / 2, displayResolutionSize.height - chapterTitle->getContentSize().height / 2 - 20);
    this->mainLayer->addChild(chapterTitle);
    
    UGMenu* mainMenu = UGMenu::create();
    mainMenu->setPosition(Point::ZERO);
    this->mainLayer->addChild(mainMenu);
    
    MenuItemSprite* backButton = Utils::createButton((char*) TXT_BACK, 16, ANI_BUTTON, ANI_BUTTON);
    backButton->setCallback(CC_CALLBACK_1(ChapterScene::backButtonClick, this));
    backButton->setPosition(gameModel->getDisplayResolutionSize().width / 2, 20 + backButton->getContentSize().height / 2);
    mainMenu->addChild(backButton);
    this->nodes->setObject(backButton, NODE_BUTTON_BACK);
    
    // create chapters
    this->chapterLayer = UGLayerColor::create();
    float totalChapterSize = gameModel->getMapData()->getLevelSize();
    float distancePerChapter = CONF_DISTANCE_PER_CHAPTER;
    float totalChapterWidth = (totalChapterSize - 1) * distancePerChapter;
    
    Face* chapterTemp = new Face();
    chapterTemp->initWithSpriteFrameName(ANI_BOX_CHAPTER);
    this->chapterTempSize = chapterTemp->getContentSize();
    
    this->chapterLayer->setContentSize(Size(totalChapterWidth + this->chapterTempSize.width, displayResolutionSize.height / 2));
    this->chapterLayer->setPosition(displayResolutionSize.width / 2 - this->chapterTempSize.width / 2, displayResolutionSize.height / 2 - this->chapterLayer->getContentSize().height / 2);
    this->chapterLayerOffset = this->chapterLayer->getPosition();
    this->mainLayer->addChild(this->chapterLayer);
    
    float posX = this->chapterTempSize.width / 2;
    float posY = this->chapterLayer->getContentSize().height / 2;
    for (int i = 0; i < totalChapterSize; i++) {
        Face* levelBox = new Face();
        if (!gameModel->checkLevelLock(i + 1)) {
            levelBox->initWithSpriteFrameName(ANI_BOX_CHAPTER);
            levelBox->setTag(i + 1);
            {
                int chapter = gameModel->getMapData()->getChapter(i + 1);
                int chapterLevel = gameModel->getMapData()->getChapterLevel(i + 1);
                char mapPath[200];
                sprintf(mapPath, DOC_BG_MAP, chapter, chapterLevel);
                
                SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
                sfc->addSpriteFramesWithFile(mapPath);
                char bg1Path[200];
                sprintf(bg1Path, ANI_MAP, chapter, chapterLevel, 1);

                Face* chapterThumb = new Face();
                chapterThumb->initWithSpriteFrameName(bg1Path);
                chapterThumb->setPosition(levelBox->getContentSize().width / 2, levelBox->getContentSize().height / 2 - 10);
                chapterThumb->setTextureRect(Rect(0, 0, 160, 115));
                levelBox->addChild(chapterThumb);
                
                string levelName = gameModel->getMapData()->getLevelName(i + 1);
                char levelNameChar[200];
                sprintf(levelNameChar, "%s", levelName.data());
                Label* levelNameLabel = Label::createWithBMFont(FONT_GAME_SMALL, levelNameChar);
                levelNameLabel->setPosition(chapterThumb->getPosition() + Point(0, chapterThumb->getContentSize().height / 2 + 12));
                levelBox->addChild(levelNameLabel);
            }
        } else {
            levelBox->setTag(-1);
            levelBox->initWithSpriteFrameName(ANI_BOX_CHAPTER_LOCK);
        }
        
        levelBox->setPosition(posX, posY);
        posX = posX + distancePerChapter;
        this->chapterLayer->addChild(levelBox);
    }
    
    // add parent button for share layer
    this->shareLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_BACK));
}

void ChapterScene::initialize() {
    this->createLayers();
    this->createLayersContent();
    
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan = CC_CALLBACK_2(ChapterScene::onTouchBegan, this);
    this->touchListener->onTouchMoved = CC_CALLBACK_2(ChapterScene::onTouchMoved, this);
    this->touchListener->onTouchEnded = CC_CALLBACK_2(ChapterScene::onTouchEnded, this);
    this->touchListener->onTouchCancelled = CC_CALLBACK_2(ChapterScene::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    
    this->listener = EventListenerKeyboard::create();
    this->listener->onKeyPressed = CC_CALLBACK_2(ChapterScene::onKeyPress, this);
    this->listener->onKeyReleased = CC_CALLBACK_2(ChapterScene::onKeyRelease, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void ChapterScene::processEvent(GameEvent* gameEvent) {
}

void ChapterScene::backButtonClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_BACK_BEGIN);
    this->fireEvent(gameEvent);
    
    this->handleFlashButton(sender);
}

void ChapterScene::updateChapterLayer(Point point) {
    this->chapterLayer->setPosition(this->chapterLayer->getPosition().x + point.x, this->chapterLayer->getPosition().y);
}

void ChapterScene::updatePageAuto() {
    GameModel* gameModel = GameModel::getInstance();
    int chapterSize = gameModel->getMapData()->getLevelSize();
    float distance = this->chapterLayerOffset.x - this->chapterLayer->getPosition().x;
    float result = distance / (this->chapterLayer->getContentSize().width / chapterSize);
    int page = floor(result);
    float redundant = result - page;
    if (redundant > 0.5f) {
        page++;
    }
    page++;
    if (page < 1) {
        page = 1;
    } else if (page > chapterSize) {
        page = chapterSize;
    }
    this->updatePage(page);
}

void ChapterScene::updatePage(int page) {
    this->currentPage = page;
    float posX = CONF_DISTANCE_PER_CHAPTER * (page - 1);
    Point floatPosition = this->chapterLayerOffset - Point(posX, 0);
    FiniteTimeAction* actionMove = MoveTo::create(0.2f, floatPosition);
    FiniteTimeAction* callbackAction = CallFunc::create(CC_CALLBACK_0(ChapterScene::onMoveChapterDone, this));
    this->chapterLayer->runAction(Sequence::create(actionMove, callbackAction, NULL));
    this->chapterMoving = true;
}

void ChapterScene::onMoveChapterDone() {
    this->isTouched = false;
    this->chapterMoving = false;
}

bool ChapterScene::onTouchBegan(Touch* touch, Event* event) {
    if (this->shareLayer->isVisible() ||
        this->chapterMoving) {
        return false;
    }
    Point point = this->convertTouchToNodeSpace(touch);
    
    this->isMoved = false;
    this->isTouched = false;
    RectBody* chapterLayerBody = new RectBody(this->chapterLayer->getContentSize().width, this->chapterLayer->getContentSize().height);
    chapterLayerBody->setPosition(this->chapterLayer->getPosition() + Point(chapterLayerBody->getWidth() / 2, chapterLayerBody->getHeight() / 2));
    
    if (chapterLayerBody->contains(point)) {
        this->isTouched = true;
        this->firstPoint = point;
        
        // update flash
        {
            Point point = this->convertTouchToNodeSpace(touch);
            point = point - this->chapterLayer->getPosition();
            
            for (int i = 0; i < this->chapterLayer->getChildren().size(); i++) {
                Face* chapterFace = dynamic_cast<Face*>(this->chapterLayer->getChildren().at(i));
                if (chapterFace != NULL) {
                    this->flashOff(chapterFace);
                    RectBody* chapterBody = new RectBody(chapterFace->getContentSize().width, chapterFace->getContentSize().height);
                    chapterBody->setPosition(chapterFace->getPosition());
                    
                    if (chapterBody->contains(point)) {
                        this->flashOn(chapterFace);
                    }
                }
            }
        }
        return true;
    }
    return false;
}

void ChapterScene::onTouchMoved(Touch* touch, Event* event) {
    Point point = this->convertTouchToNodeSpace(touch);

    Point vectorMove = Point::ZERO;
    if (!this->isMoved) {
        vectorMove = point - this->firstPoint;
    } else {
        vectorMove = point - this->movePoint;
    }
    this->updateChapterLayer(vectorMove);
    this->movePoint = point;
    this->isMoved = true;
    
    // update flash
    {
        Point point = this->convertTouchToNodeSpace(touch);
        point = point - this->chapterLayer->getPosition();
        
        for (int i = 0; i < this->chapterLayer->getChildren().size(); i++) {
            Face* chapterFace = dynamic_cast<Face*>(this->chapterLayer->getChildren().at(i));
            if (chapterFace != NULL) {
                this->flashOff(chapterFace);
                RectBody* chapterBody = new RectBody(chapterFace->getContentSize().width, chapterFace->getContentSize().height);
                chapterBody->setPosition(chapterFace->getPosition());
                
                if (chapterBody->contains(point)) {
                    this->flashOn(chapterFace);
                }
            }
        }
    }
}

void ChapterScene::onTouchEnded(Touch* touch, Event* event) {
    GameModel* gameModel = GameModel::getInstance();
    this->updatePageAuto();
    
    Point point = this->convertTouchToNodeSpace(touch);
    bool moveTooFar = false;
    float dx = point.x - this->firstPoint.x;
    float dy = point.y - this->firstPoint.y;
    if (abs(dx) > CONF_TOUCH_RANGE || abs(dy) > CONF_TOUCH_RANGE) {
        moveTooFar = true;
    }
    point = point - this->chapterLayer->getPosition();
    
    for (int i = 0; i < this->chapterLayer->getChildren().size(); i++) {
        Face* chapterFace = dynamic_cast<Face*>(this->chapterLayer->getChildren().at(i));
        if (chapterFace != NULL) {
            RectBody* chapterBody = new RectBody(chapterFace->getContentSize().width, chapterFace->getContentSize().height);
            chapterBody->setPosition(chapterFace->getPosition());
            
            if (chapterBody->contains(point)) {
                if (!moveTooFar && chapterFace->getTag() != -1) {
                    if (!gameModel->checkLevelRequireSocial(i + 1)) {
                        this->shareLayer->show(TXT_REQUIRE_SHARE);
                        GameEvent* gameEvent = new GameEvent();
                        gameEvent->setEventCode(EVT_SHOW_SHARE_REQUIRED);
                        this->fireEvent(gameEvent);
                    } else {
                        DataEvent* gameEvent = new DataEvent();
                        gameEvent->setEventCode(EVT_START_PLAY);
                        gameEvent->setArgumentInt(chapterFace->getTag());
                        this->fireEvent(gameEvent);
                        
                        this->_eventDispatcher->removeEventListener(this->touchListener);
                    }
                }
                this->flash(chapterFace);
                break;
            }
        }
    }
}

void ChapterScene::showCongratulationMessage() {
    this->shareLayer->show(TXT_CONRATULATION_SHARE);
}

void ChapterScene::onTouchCancelled(Touch* touch, Event* event) {
    this->onTouchEnded(touch, event);
}

void ChapterScene::update(float dt) {
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_BACK);
        this->checkFlash(button);
    }
}

void ChapterScene::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
}

void ChapterScene::onKeyRelease(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    Director::getInstance()->getEventDispatcher()->removeEventListener(this->listener);
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
        GameEvent* gameEvent = new GameEvent();
        gameEvent->setEventCode(EVT_BACK_BEGIN);
        this->fireEvent(gameEvent);
    }
}