#include "GameScene.h"

GameScene::GameScene() {
    this->initialize();
}

GameScene::~GameScene() {
}

void GameScene::onEnter() {
    AbstractScene::onEnter();
}

void GameScene::onExit() {
    AbstractScene::onExit();
    this->removeAllChildren();
}

void GameScene::createLayers() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    UGLayerColor* colorLayer = UGLayerColor::create();
    colorLayer->initWithColor(Color4B(255, 255, 255, 255));
    this->addChild(colorLayer);
    
    this->backgroundLayer = UGLayer::create();
    this->addChild(this->backgroundLayer);
    
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
    
    this->mainMenu = UGMenu::create();
    this->mainMenu->setPosition(Point::ZERO);
    this->addChild(this->mainMenu);
    
    UGLayer* joystickLayer = UGLayer::create();
	joystickLayer->setPosition(Point::ZERO);
    joystickLayer->setContentSize(Size(displayResolutionSize.width / 2, displayResolutionSize.height));
	joystickLayer->setVisible(true);
	this->addChild(joystickLayer);
    
    this->controllerLayer = new ControllerLayer(this);
    joystickLayer->addChild(this->controllerLayer);
    
    this->padLayer = new PadLayer(this);
    this->addChild(this->padLayer);
    
    this->guiLayer = UGLayer::create();
    this->addChild(this->guiLayer);
    
    this->pauseLayer = UGLayer::create();
    this->addChild(this->pauseLayer);
}

void GameScene::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    // Main menu
    {
        MenuItemSprite* pauseButton = Utils::createButton(ANI_PAUSE, ANI_PAUSE);
        pauseButton->setCallback(CC_CALLBACK_1(GameScene::onPauseButtonClick, this));
        pauseButton->setPosition(Point(displayResolutionSize.width - pauseButton->getContentSize().width / 2 - 10,
                                       displayResolutionSize.height - pauseButton->getContentSize().height / 2 - 10));
        this->mainMenu->addChild(pauseButton);
        this->nodes->setObject(pauseButton, NODE_BUTTON_PAUSE);
    }
    
    // Gui layer
    {
        // hp
        float barPaddingY = 10;
        float barOffsetY = gameModel->getDisplayResolutionSize().height - barPaddingY;
        float paddingBarLeft = 130;
        this->hpBar = new StatusBar();
        this->hpBar->initWithParameter((char*) ANI_BAR_HP, (char*) ANI_BAR_HP_CONTENT);
        this->hpBar->setAnchorPoint(Point(1, 0.5f));
        barOffsetY = barOffsetY - this->hpBar->getContentSize().height / 2 - barPaddingY;
        this->hpBar->setPosition(paddingBarLeft, barOffsetY);
        barOffsetY = barOffsetY - this->hpBar->getContentSize().height / 2;
        this->hpBar->setValue(1);
        this->guiLayer->addChild(this->hpBar);
        {
            Face* hpIcon = new Face();
            hpIcon->initWithFile(ANI_ICON_HP);
            hpIcon->setPosition(this->hpBar->getPosition() - Point(this->hpBar->getContentSize().width + 10 + hpIcon->getContentSize().width / 2, 0));
            this->guiLayer->addChild(hpIcon);
        }
        
        this->angryBar = new StatusBar();
        this->angryBar->initWithParameter((char*) ANI_BAR_ANGRY, (char*) ANI_BAR_ANGRY_CONTENT);
        this->angryBar->setAnchorPoint(Point(1, 0.5f));
        barOffsetY = barOffsetY - this->angryBar->getContentSize().height / 2 - barPaddingY;
        this->angryBar->setPosition(paddingBarLeft, barOffsetY);
        barOffsetY = barOffsetY - this->angryBar->getContentSize().height / 2;
        this->angryBar->setValue(0);
        this->guiLayer->addChild(this->angryBar);
        {
            Face* angryIcon = new Face();
            angryIcon->initWithFile(ANI_ICON_ANGRY);
            angryIcon->setPosition(this->angryBar->getPosition() - Point(this->angryBar->getContentSize().width + 10 + angryIcon->getContentSize().width / 2, 0));
            this->guiLayer->addChild(angryIcon);
        }
        
        this->weaponBar = new StatusBar();
        this->weaponBar->initWithParameter((char*) ANI_BAR_WP, (char*) ANI_BAR_WP_CONTENT);
        this->weaponBar->setAnchorPoint(Point(1, 0.5f));
        this->weaponBar->setVisible(false);
        barOffsetY = barOffsetY - this->weaponBar->getContentSize().height / 2 - barPaddingY;
        this->weaponBar->setPosition(paddingBarLeft, barOffsetY);
        barOffsetY = barOffsetY - this->weaponBar->getContentSize().height / 2;
        this->weaponBar->setValue(0);
        this->guiLayer->addChild(this->weaponBar);
        {
            Face* wpIcon = new Face();
            wpIcon->initWithFile(ANI_ICON_WP);
            wpIcon->setPosition(this->weaponBar->getPosition() - Point(this->weaponBar->getContentSize().width + 10 + wpIcon->getContentSize().width / 2, 0));
            wpIcon->setVisible(false);
            this->guiLayer->addChild(wpIcon);
            this->nodes->setObject(wpIcon, NODE_WP_ICON);
        }
        
        // game time
        this->gameTimeLabel = Label::createWithBMFont(FONT_GAME_BIG, "");
        this->gameTimeLabel->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height - 40));
        this->guiLayer->addChild(this->gameTimeLabel);
        
        // score
        MenuItemSprite* buttonPause = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_PAUSE);
        float offsetX = buttonPause->getPosition().x - buttonPause->getContentSize().width / 2;
        float offsetY = buttonPause->getPosition().y;
        Label* scoreCaption = Label::createWithBMFont(FONT_GAME_SMALL, TXT_SCORE);
        scoreCaption->setAnchorPoint(Point(1, 0.5f));
        scoreCaption->setHorizontalAlignment(TextHAlignment::RIGHT);
        scoreCaption->setPosition(offsetX - 10, offsetY + 10);
        this->guiLayer->addChild(scoreCaption);
        
        this->scoreLabel = Label::createWithBMFont(FONT_GAME_SMALL, "0");
        this->scoreLabel->setAnchorPoint(Point(1, 0.5f));
        this->scoreLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
        this->scoreLabel->setPosition(offsetX - 10, offsetY - 10);
        this->guiLayer->addChild(this->scoreLabel);
    }
    
    // Pause layer
    {
        // bg
        Face* backgroundFade = new Face();
        backgroundFade->initWithFile(ANI_BACKGROUND_FADE);
        backgroundFade->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        backgroundFade->setOpacity(0);
        this->pauseLayer->addChild(backgroundFade);
        this->nodes->setObject(backgroundFade, NODE_FADE_BG);
        {
            FiniteTimeAction* fadeIn = FadeIn::create(0.5f);
            backgroundFade->registerState(STATE_FADE_IN, fadeIn);
            
            FiniteTimeAction* fadeOut = FadeOut::create(0.5f);
            backgroundFade->registerState(STATE_FADE_OUT, fadeOut);
        }
        
        // board
        Face* menuFace = new Face();
        menuFace->initWithSpriteFrameName(ANI_BOX_PAUSE);
        menuFace->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height + 500));
        this->pauseLayer->addChild(menuFace);
        this->nodes->setObject(menuFace, NODE_PAUSE_BOARD);
        Size menuFaceSize = menuFace->getContentSize();
        
        UGMenu* pauseMenu = UGMenu::create();
        pauseMenu->setPosition(Point::ZERO);
        menuFace->addChild(pauseMenu);
        
        // option layer
        OptionLayer* optionLayer = new OptionLayer(this);
        optionLayer->setVisible(false);
        this->pauseLayer->addChild(optionLayer);
        this->nodes->setObject(optionLayer, NODE_OPTION_LAYER);
        
        {
            FiniteTimeAction* move1 = MoveTo::create(0, menuFace->getPosition());
            FiniteTimeAction* move2 = MoveTo::create(0.5f, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
            FiniteTimeAction* move3 = CallFunc::create(CC_CALLBACK_0(GameScene::onPauseBoardMoveDownDone, this));
            FiniteTimeAction* move123 = Sequence::create(move1, move2, move3, NULL);
            menuFace->registerState(STATE_MOVE_DOWN, move123);
            
            FiniteTimeAction* move4 = MoveTo::create(0, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
            FiniteTimeAction* move5 = MoveTo::create(0.5f, menuFace->getPosition());
            FiniteTimeAction* move6 = CallFunc::create(CC_CALLBACK_0(GameScene::onPauseBoardMoveUpDone, this));
            FiniteTimeAction* move456 = Sequence::create(move4, move5, move6, NULL);
            menuFace->registerState(STATE_MOVE_UP, move456);
            
            MenuItemSprite* resumeButton = Utils::createButton((char*) TXT_RESUME, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
            resumeButton->setCallback(CC_CALLBACK_1(GameScene::onResumeButtonClick, this));
            resumeButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25 + 50));
            resumeButton->setEnabled(false);
            pauseMenu->addChild(resumeButton);
            optionLayer->getParentButtons()->pushBack(resumeButton);
            this->nodes->setObject(resumeButton, NODE_BUTTON_RESUME);
            
            MenuItemSprite* optionButton = Utils::createButton((char*) TXT_OPTIONS, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
            optionButton->setCallback(CC_CALLBACK_1(GameScene::onOptionsButtonClick, this));
            optionButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25));
            optionButton->setEnabled(false);
            pauseMenu->addChild(optionButton);
            optionLayer->getParentButtons()->pushBack(optionButton);
            this->nodes->setObject(optionButton, NODE_BUTTON_OPTION);
            
            MenuItemSprite* quitButton = Utils::createButton((char*) TXT_QUIT_GAME, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
            quitButton->setCallback(CC_CALLBACK_1(GameScene::onQuitButtonClick, this));
            quitButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25 - 50));
            quitButton->setEnabled(false);
            pauseMenu->addChild(quitButton);
            optionLayer->getParentButtons()->pushBack(quitButton);
            this->nodes->setObject(quitButton, NODE_BUTTON_QUIT_GAME);
        }
        
        Label* levelLabel = Label::createWithBMFont(FONT_GAME_SMALL, "");
        __Dictionary* levelLabelActions = __Dictionary::create();
        levelLabel->setUserObject(levelLabelActions);
        levelLabel->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height + 200));
        {
            FiniteTimeAction* move1 = MoveTo::create(0, levelLabel->getPosition());
            FiniteTimeAction* move2 = MoveTo::create(0.5f, Point(gameModel->getDisplayResolutionSize().width / 2,
                                                                 gameModel->getDisplayResolutionSize().height / 2 +
                                                                 menuFace->getContentSize().height / 2 +
                                                                 20));
            FiniteTimeAction* move12 = Sequence::create(move1, move2, NULL);
            levelLabelActions->setObject(move12, STATE_MOVE_DOWN);
            
            FiniteTimeAction* move3 = MoveTo::create(0, Point(gameModel->getDisplayResolutionSize().width / 2,
                                                              gameModel->getDisplayResolutionSize().height / 2 +
                                                              menuFace->getContentSize().height / 2 +
                                                              20));
            FiniteTimeAction* move4 = MoveTo::create(0.5f, levelLabel->getPosition());
            
            FiniteTimeAction* move34 = Sequence::create(move3, move4, NULL);
            levelLabelActions->setObject(move34, STATE_MOVE_UP);
        }
        this->pauseLayer->addChild(levelLabel);
        this->nodes->setObject(levelLabel, NODE_CHAPTER_LEVEL_LABEL);
    }
}

void GameScene::initialize() {
    this->createLayers();
    this->createLayersContent();
    
    GameModel* gameModel = GameModel::getInstance();
    // Register model event listener
    this->gameEventListener = new GameEventListener();
    this->gameEventListener->setHandlerObject(this);
    this->gameEventListener->setHandlerFunction(handler_function(GameScene::processEvent));
    gameModel->addEventListener(this->gameEventListener);
    
    this->touchListener = EventListenerTouchAllAtOnce::create();
    this->touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    this->touchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
    this->touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    
    this->listener = EventListenerKeyboard::create();
    this->listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPress, this);
    this->listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyRelease, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void GameScene::processEvent(GameEvent* gameEvent) {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    switch (gameEvent->getEventCode()) {
        case EVT_REPLAY_MAP: {            
            gameModel->removeEventListener(this->gameEventListener);
            this->_eventDispatcher->removeEventListener(this->touchListener);
            this->_eventDispatcher->removeEventListener(this->listener);
            break;
        }
        case EVT_NEXT_MAP: {
            gameModel->removeEventListener(this->gameEventListener);
            this->_eventDispatcher->removeEventListener(this->touchListener);
            this->_eventDispatcher->removeEventListener(this->listener);
            
            if (gameModel->isLastMap()) {
                DataEvent* gameEvent = new DataEvent();
                gameEvent->setEventCode(EVT_COMPLETED_GAME);
                this->fireEvent(gameEvent);
                return;
            }
            break;
        }
        case EVT_MAP_CREATED: {
            // maps
            int level = gameModel->getCurrentLevel()->getLevel();
            int chapter = gameModel->getMapData()->getChapter(level);
            int chapterLevel = gameModel->getMapData()->getChapterLevel(level);
            char mapPath[200];
            sprintf(mapPath, DOC_BG_MAP, chapter, chapterLevel);
            
            SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
            sfc->addSpriteFramesWithFile(mapPath);
            
            char bg1Path[200];
            sprintf(bg1Path, ANI_MAP, chapter, chapterLevel, 1);
            Face* bg1Face = new Face();
            bg1Face->initWithSpriteFrameName(bg1Path);
            bg1Face->setAnchorPoint(Point::ZERO);
            bg1Face->setPosition(Point::ZERO);
            bg1Face->setScale(CONF_SCALE_GAME);
            bg1Face->getTexture()->setAliasTexParameters();
            this->mainLayer->addChild(bg1Face);
            Size bg1FaceSize = bg1Face->getContentSize();
            
            if (chapterLevel % 4 != 0) {
                char bg2Path[200];
                sprintf(bg2Path, ANI_MAP, chapter, chapterLevel, 2);
                Face* bg2Face = new Face();
                bg2Face->initWithSpriteFrameName(bg2Path);
                bg2Face->setAnchorPoint(Point::ZERO);
                bg2Face->setPosition(Point(bg1FaceSize.width * CONF_SCALE_GAME, 0));
                bg2Face->setScale(CONF_SCALE_GAME);
                bg2Face->getTexture()->setAliasTexParameters();
                this->mainLayer->addChild(bg2Face);
                Size bg2FaceSize = bg2Face->getContentSize();
                
                
                // create endzone
                if (gameModel->getCurrentLevel()->getStops()->size() > 0) {
                    float totalMapWidth = bg2Face->getPosition().x + bg2FaceSize.width * CONF_SCALE_GAME;
                    Face* endZone = new Face();
                    endZone->initWithFile(ANI_END_ZONE);
                    endZone->setPosition(totalMapWidth - endZone->getContentSize().width / 2, CONF_END_ZONE_Y);
                    this->mainLayer->addChild(endZone);
                    {
                        FadeTo* fadeIn = FadeTo::create(1, 127);
                        FadeTo* fadeOut = FadeTo::create(1, 255);
                        Sequence* pulseSequence = Sequence::create(fadeIn, fadeOut, NULL);
                        RepeatForever* repeat = RepeatForever::create(pulseSequence);
                        endZone->runAction(repeat);
                    }
                    DataEvent* dataEvent = new DataEvent();
                    dataEvent->setEventCode(EVT_CREATE_END_ZONE);
                    dataEvent->setArgumentPoint(Point(endZone->getContentSize().width, endZone->getContentSize().height));
                    this->fireEvent(dataEvent);
                }

            }
            
            // charactes
            this->addFacesToLayer(gameModel->getCurrentLevel()->getMan(), this->mainLayer, 0);
            
            // chapter level
            char levelText[200];
            sprintf(levelText, TXT_CHAPTER_LEVEL, gameModel->getMapData()->getChapter(level), gameModel->getMapData()->getChapterLevel(level));
            Label* chapterLevelLabel = (Label*) this->nodes->objectForKey(NODE_CHAPTER_LEVEL_LABEL);
            chapterLevelLabel->setString(levelText);
            
            // zombies
            for (int i = 0; i < gameModel->getCurrentLevel()->getZombies()->size(); i++) {
                Zombie* zombie = gameModel->getCurrentLevel()->getZombies()->at(i);
                
                Face* charFace = zombie->getFace((char*) FACE_CHAR);
                FiniteTimeAction* fadeInCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::fadeInCallback, this));
                FiniteTimeAction* delayTimeAction1 = DelayTime::create(0.05f);
                FiniteTimeAction* fadeOutCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::fadeOutCallback, this));
                FiniteTimeAction* delayTimeAction2 = DelayTime::create(0.05f);
                ActionInterval* sequenceAction = Sequence::create(fadeInCallback, delayTimeAction1, fadeOutCallback, delayTimeAction2, NULL);
                FiniteTimeAction* repeatAction = RepeatForever::create(sequenceAction);
                charFace->registerState(STATE_ALMOST_DIE, repeatAction);
                
                this->addFacesToLayer(zombie, this->mainLayer, 0);
            }
            
            // barrels
            for (int i = 0; i < gameModel->getCurrentLevel()->getBarrels()->size(); i++) {
                Barrel* barrel = gameModel->getCurrentLevel()->getBarrels()->at(i);
                this->addFacesToLayer(barrel, this->mainLayer, 0);
            }
            
            //elevators
            for (int i = 0; i < gameModel->getCurrentLevel()->getElevators()->size(); i++) {
                Elevator* elevator = gameModel->getCurrentLevel()->getElevators()->at(i);
                this->addFacesToLayer(elevator, this->mainLayer, 0);
            }
            
            //boss
            if (gameModel->getCurrentLevel()->getBoss() != NULL) {
                this->addFacesToLayer(gameModel->getCurrentLevel()->getBoss(), this->mainLayer, 0);
            }
            
            //boss HP
            if (gameModel->getCurrentLevel()->getBoss() != NULL) {
                this->bossBar = new StatusBar();
                this->bossBar->initWithParameter((char*) ANI_BAR_HP, (char*) ANI_BAR_HP_CONTENT);
                this->bossBar->setValue(1);
                this->bossBar->setPosition(Point(gameModel->getDisplayResolutionSize().width/2, this->gameTimeLabel->getPosition().y - this->gameTimeLabel->getContentSize().height/2 - this->bossBar->getContentSize().height/2 - 45));
                this->guiLayer->addChild(this->bossBar);
            }
            break;
        }
        case EVT_NEW_HIT_PUNCH: {
            DataEvent* dataEvent = (DataEvent*) gameEvent;
            AnimatedObject* hitObject = new AnimatedObject();
            hitObject->initFaces(KEY_FX, gameModel->getAnimations());
            
            Face* faceChar = hitObject->getFace((char*) FACE_CHAR);
            faceChar->setPosition(dataEvent->getArgumentPoint());
            faceChar->setScale(CONF_FAKE_1);
            faceChar->setPosition(faceChar->getPosition() + Point(0, CONF_MAN_MIN_VECTOR_Y));
            ActionInterval* actionHitFlash2 = (ActionInterval*) faceChar->getState(STATE_HIT_FLASH_2);
            float duration = actionHitFlash2->getDuration();
            faceChar->runAction(actionHitFlash2);
            this->mainLayer->addChild(faceChar, gameModel->getDisplayResolutionSize().height);
            
            FiniteTimeAction* delayTimeAction = DelayTime::create(duration);
            FiniteTimeAction* callbackFunction = CallFuncN::create(CC_CALLBACK_1(GameScene::animationCallback, this));
            FiniteTimeAction* sequenceAction = Sequence::create(delayTimeAction, callbackFunction, NULL);
            faceChar->runAction(sequenceAction);
            
            break;
        }
        case EVT_STAR_ROTATE: {
            DataEvent* dataEvent = (DataEvent*) gameEvent;
            AnimatedObject* hitObject = new AnimatedObject();
            hitObject->initFaces(KEY_FX, gameModel->getAnimations());
            
            Face* faceChar = hitObject->getFace((char*) FACE_CHAR);
            faceChar->setPosition(dataEvent->getArgumentPoint());
            faceChar->setScale(CONF_FAKE_1);
            faceChar->setPosition(dataEvent->getArgumentPoint());
            ActionInterval* starRotateAction = (ActionInterval*) faceChar->getState(STATE_STARS);
            faceChar->runAction(starRotateAction);
            faceChar->setOpacity(0);
            this->mainLayer->addChild(faceChar, gameModel->getDisplayResolutionSize().height);
            
            FiniteTimeAction* delayTimeAction1 = DelayTime::create(dataEvent->getArgumentFloat());
            FiniteTimeAction* fadeInAction = FadeIn::create(0);
            FiniteTimeAction* delayTimeAction2 = DelayTime::create(dataEvent->getArgumentFloat2());
            FiniteTimeAction* callbackFunction = CallFuncN::create(CC_CALLBACK_1(GameScene::animationCallback, this));
            FiniteTimeAction* sequenceAction = Sequence::create(delayTimeAction1, fadeInAction, delayTimeAction2, callbackFunction, NULL);
            faceChar->runAction(sequenceAction);
            
            break;
        }
        case EVT_ZOMBIE_ALMOST_DIE: {
            DataEvent* dataEvent = (DataEvent*) gameEvent;
            Zombie* zombie = (Zombie*) dataEvent->getArgumentObject();
            Face* charFace = zombie->getFace((char*) FACE_CHAR);
            charFace->setUserObject(zombie);

            FiniteTimeAction* firstDelayTimeAction = DelayTime::create(dataEvent->getArgumentFloat());
            FiniteTimeAction* fadeInCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::fadeInCallback, this));
            FiniteTimeAction* delayTimeAction = DelayTime::create(0.2f);
            FiniteTimeAction* fadeOutCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::fadeOutCallback, this));
            FiniteTimeAction* almostDieCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::almostDieCallback, this));

            ActionInterval* sequenceAction = Sequence::create(firstDelayTimeAction,
                                                              fadeOutCallback->clone(),
                                                              delayTimeAction->clone(),
                                                              fadeInCallback->clone(),
                                                              delayTimeAction->clone(),
                                                              fadeOutCallback->clone(),
                                                              delayTimeAction->clone(),
                                                              almostDieCallback,
                                                              NULL);
            charFace->runAction(sequenceAction);
            
            
            float totalTime = dataEvent->getArgumentFloat() + (0.2f * 3) + dataEvent->getArgumentFloat2();
            FiniteTimeAction* delayToDestroy = DelayTime::create(totalTime);
            FiniteTimeAction* destroyCallback = CallFuncN::create(CC_CALLBACK_1(GameScene::selfDestroyCallback, this));
            FiniteTimeAction* destroySequence = Sequence::create(delayToDestroy, destroyCallback, NULL);
            charFace->runAction(destroySequence);
            
            break;
        }
        case EVT_NEXT_CHECK_POINT: {
            Face* nextArrowFace = new Face();
            nextArrowFace->initWithFile(ANI_NEXT_ARROW);
            nextArrowFace->setVisible(false);
            nextArrowFace->setPosition(displayResolutionSize.width - nextArrowFace->getContentSize().width / 2 - 20, displayResolutionSize.height / 2);
            this->guiLayer->addChild(nextArrowFace, displayResolutionSize.height);
            
            FiniteTimeAction* blinkAction = Blink::create(4, 4);
            FiniteTimeAction* callbackAction = CallFuncN::create(CC_CALLBACK_1(GameScene::animationCallback, this));
            FiniteTimeAction* sequenceAction = Sequence::create(blinkAction, callbackAction, NULL);
            nextArrowFace->runAction(sequenceAction);
            break;
        }
        case EVT_NEW_SHOT: {
            DataEvent* dataEvent = (DataEvent*) gameEvent;
            this->addFacesToLayer(dataEvent->getArgumentObject(), this->mainLayer, displayResolutionSize.height);
            break;
        }
        case EVT_DELETE_SHOT: {
            DataEvent* dataEvent = (DataEvent*) gameEvent;
            this->removeFaces(dataEvent->getArgumentObject());
            break;
        }
        default: {
            break;
        }
    }
    this->fireEvent(gameEvent);
}

void GameScene::selfDestroyCallback(Ref* sender) {
    Face* face = (Face*) sender;
    Zombie* zombie = (Zombie*) face->getUserObject();
    this->removeFaces(zombie);
}

void GameScene::animationCallback(Ref* sender) {
    Face* face = (Face*) sender;
    face->removeFromParentAndCleanup(true);
}

void GameScene::fadeInCallback(Ref* sender) {
    Face* face = (Face*) sender;
    face->setOpacity(255);
}

void GameScene::fadeOutCallback(Ref* sender) {
    Face* face = (Face*) sender;
    face->setOpacity(100);
}
                                                              
void GameScene::almostDieCallback(Ref* sender) {
    Face* face = (Face*) sender;
    face->runAction(face->getState(STATE_ALMOST_DIE));
}

void GameScene::addFacesToLayer(Obj* object, Layer* layer, int index) {
    for (int i = 0; i < object->getFaces()->count(); i++) {
        Face* node = (Face*) object->getFaces()->getObjectAtIndex(i);
        node->setPosition(object->getPosition() + node->getOOVector());
        node->setAnchorPoint(object->getBody()->getAnchorPoint());
        node->setFlippedX(object->getBody()->isFlipX());
        node->setScale(object->getBody()->getScale());
        if (strcmp(node->getName(), FACE_SHADOW) == 0) {
            node->setScale(object->getBody()->getScale() * 0.25f);
        }
        node->setVisible(object->getBody()->isVisible());
        layer->addChild(node, index);
    }
}

void GameScene::removeFaces(Obj* object) {
    for (int i = 0; i < object->getFaces()->count(); i++) {
        Node* node = (Node*) object->getFaces()->getObjectAtIndex(i);
        node->removeFromParentAndCleanup(true);
    }
}

void GameScene::update(float dt) {
    GameModel* gameModel = GameModel::getInstance();
    this->mainLayer->setPosition(Point(-gameModel->getCurrentLevel()->getAirX(), 0));
    
    if (gameModel->getGameState() == GS_PLAY ||
        gameModel->getGameState() == GS_REPLAY) {
        
        // update time
        char gameTimeChar[200];
        sprintf(gameTimeChar, "%d", (int) gameModel->getCurrentLevel()->getGameTime());
        this->gameTimeLabel->setString(gameTimeChar);
        
        // update bars
        float hpPercent = gameModel->getCurrentLevel()->getMan()->getHP() / CONF_MAN_HP;
        this->hpBar->setValue(hpPercent);
        
        if (gameModel->getCurrentLevel()->getBoss() != NULL) {
            float bossPercent = gameModel->getCurrentLevel()->getBoss()->getHP() / CONF_MAN_HP;
            this->bossBar->setValue(bossPercent);
        }
        
        float apPercent = gameModel->getCurrentLevel()->getMan()->getAP() / CONF_MAN_ANGRY;
        this->angryBar->setValue(apPercent);
        
        float wpPercent = gameModel->getCurrentLevel()->getMan()->getWP() / CONF_MAN_WP;
        this->weaponBar->setValue(wpPercent);
        Face* wpIcon = (Face*) this->nodes->objectForKey(NODE_WP_ICON);
        if (wpPercent <= 0) {
            this->weaponBar->setVisible(false);
            wpIcon->setVisible(false);
        } else {
            this->weaponBar->setVisible(true);
            wpIcon->setVisible(true);
        }
        
        // update score
        char scoreChar[200];
        sprintf(scoreChar, "%d", gameModel->getCurrentLevel()->getScore());
        string scoreString = scoreChar;
        int scoreStringLength = scoreString.length();
        int numberComma = (scoreStringLength - 1) / 3;
        int currentCounter = 0;
        while (currentCounter < numberComma) {
            string newString = scoreString.insert(scoreString.length() - (3 + currentCounter + (3 * currentCounter)), ",");
            scoreString = newString;
            currentCounter++;
        }
        
        this->scoreLabel->setString(scoreString);
        
        // update index
        for (int i = 0; i < gameModel->getCurrentLevel()->getZombies()->size(); i++) {
            Zombie* zombie = gameModel->getCurrentLevel()->getZombies()->at(i);
            for (int j = 0; j < zombie->getFaces()->count(); j++) {
                Face* face = (Face*) zombie->getFaces()->getObjectAtIndex(j);
                if (strcmp(face->getName(), FACE_SHADOW) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - zombie->getPosition().y);
                }
                if (strcmp(face->getName(), FACE_CHAR) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - zombie->getPosition().y);
                }
            }
        }
        
        for (int i = 0; i < gameModel->getCurrentLevel()->getBarrels()->size(); i++) {
            Barrel* barrel = gameModel->getCurrentLevel()->getBarrels()->at(i);
            for (int j = 0; j < barrel->getFaces()->count(); j++) {
                Face* face = (Face*) barrel->getFaces()->getObjectAtIndex(j);
                if (strcmp(face->getName(), FACE_SHADOW) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - barrel->getPosition().y);
                }
                if (strcmp(face->getName(), FACE_MAIN) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - barrel->getPosition().y);
                }
            }
        }
        
        Man* man = gameModel->getCurrentLevel()->getMan();
        for (int j = 0; j < man->getFaces()->count(); j++) {
            Face* face = (Face*) man->getFaces()->getObjectAtIndex(j);
            if (strcmp(face->getName(), FACE_SHADOW) == 0) {
                face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - man->getPosition().y);
            }
            if (strcmp(face->getName(), FACE_CHAR) == 0) {
                face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - man->getPosition().y);
            }
        }
        
        if (gameModel->getCurrentLevel()->getBoss() != NULL) {
            Zombie* boss = gameModel->getCurrentLevel()->getBoss();
            for (int j = 0; j < boss->getFaces()->count(); j++) {
                Face* face = (Face*) boss->getFaces()->getObjectAtIndex(j);
                if (strcmp(face->getName(), FACE_SHADOW) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - boss->getPosition().y);
                }
                if (strcmp(face->getName(), FACE_CHAR) == 0) {
                    face->getParent()->reorderChild(face, gameModel->getDisplayResolutionSize().height - boss->getPosition().y);
                }
            }
        }
    }
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event) {
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event) {
}

void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event) {
}

void GameScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event) {
    this->onTouchesEnded(touches, event);
}

void GameScene::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
}

void GameScene::onKeyRelease(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
    }
}

void GameScene::onPauseBoardMoveDownDone() {
    MenuItemSprite* resumeButton = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RESUME);
    resumeButton->setEnabled(true);
    
    MenuItemSprite* optionButton = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_OPTION);
    optionButton->setEnabled(true);
    
    MenuItemSprite* quitGameButton = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_QUIT_GAME);
    quitGameButton->setEnabled(true);
}

void GameScene::onPauseBoardMoveUpDone() {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_RESUME_GAME);
    this->fireEvent(gameEvent);
    
    MenuItemSprite* pauseButton = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_PAUSE);
    pauseButton->setEnabled(true);
}

void GameScene::onPauseButtonClick(Ref* sender) {
    if (sender != NULL) {
        MenuItemSprite* button = (MenuItemSprite*) sender;
        button->setEnabled(false);
    }
    
    GameModel* gameModel = GameModel::getInstance();
    if (gameModel->getGameState() == GS_PLAY) {
        GameEvent* gameEvent = new GameEvent();
        gameEvent->setEventCode(EVT_PAUSE_GAME);
        this->fireEvent(gameEvent);
        
        Face* backgroundFade = (Face*) this->nodes->objectForKey(NODE_FADE_BG);
        backgroundFade->setState(backgroundFade->getState(STATE_FADE_IN));
        
        Face* pauseBoard = (Face*) this->nodes->objectForKey(NODE_PAUSE_BOARD);
        pauseBoard->setState(pauseBoard->getState(STATE_MOVE_DOWN));
        
        Label* pauseLabel = (Label*) this->nodes->objectForKey(NODE_CHAPTER_LEVEL_LABEL);
        __Dictionary* pauseLabelActions = (__Dictionary*) pauseLabel->getUserObject();
        pauseLabel->runAction((Action*) pauseLabelActions->objectForKey(STATE_MOVE_DOWN));
    }
}

void GameScene::onResumeButtonClick(Ref* sender) {
    if (sender != NULL) {
        MenuItemSprite* button = (MenuItemSprite*) sender;
        button->setEnabled(false);
        this->handleFlashButton(sender);
    }
    
    Face* backgroundFade = (Face*) this->nodes->objectForKey(NODE_FADE_BG);
    backgroundFade->setState(backgroundFade->getState(STATE_FADE_OUT));
    
    Face* pauseBoard = (Face*) this->nodes->objectForKey(NODE_PAUSE_BOARD);
    pauseBoard->setState(pauseBoard->getState(STATE_MOVE_UP));
    
    Label* pauseLabel = (Label*) this->nodes->objectForKey(NODE_CHAPTER_LEVEL_LABEL);
    __Dictionary* pauseLabelActions = (__Dictionary*) pauseLabel->getUserObject();
    pauseLabel->runAction((Action*) pauseLabelActions->objectForKey(STATE_MOVE_UP));
}

void GameScene::onOptionsButtonClick(Ref* sender) {
    OptionLayer* optionLayer = (OptionLayer*) this->nodes->objectForKey(NODE_OPTION_LAYER);
    optionLayer->show();
    
    this->handleFlashButton(sender);
}

void GameScene::onQuitButtonClick(Ref* sender) {
    MenuItemSprite* button = (MenuItemSprite*) sender;
    button->setEnabled(false);
    
    GameModel* gameModel = GameModel::getInstance();
    gameModel->removeEventListener(this->gameEventListener);
    this->_eventDispatcher->removeEventListener(this->touchListener);
    this->_eventDispatcher->removeEventListener(this->listener);
    
    DataEvent* gameEvent = new DataEvent();
    gameEvent->setEventCode(EVT_QUIT_GAME);
    this->fireEvent(gameEvent);
    
    this->handleFlashButton(sender);
}

