#include "BeginScene.h"

BeginScene::BeginScene() {
    this->listener = NULL;
    this->initialize();
}

BeginScene::~BeginScene() {
}

void BeginScene::onEnter() {
    AbstractScene::onEnter();
}

void BeginScene::onExit() {
    AbstractScene::onExit();
    this->removeAllChildren();
}

void BeginScene::createLayers() {
    this->backgroundLayer = UGLayer::create();
    this->addChild(this->backgroundLayer);
    
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
    
    this->mainMenu = UGMenu::create();
    this->mainMenu->setPosition(Point::ZERO);
    this->addChild(this->mainMenu);
    
    this->optionLayer = new OptionLayer(this);
    this->optionLayer->setVisible(false);
    this->addChild(this->optionLayer);
    
    this->confirmLayer = new ConfirmLayer(this);
    this->confirmLayer->setVisible(false);
    this->addChild(this->confirmLayer);
}

void BeginScene::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    Face* backgroundFace = new Face();
    backgroundFace->initWithFile(ANI_BACKGROUND);
    backgroundFace->setScale(CONF_FAKE_1);
    backgroundFace->setPosition(displayResolutionSize.width / 2, displayResolutionSize.height / 2);
    this->backgroundLayer->addChild(backgroundFace);
    
    Face* logoFace = new Face();
    logoFace->initWithSpriteFrameName(ANI_LOGO);
    logoFace->setPosition(displayResolutionSize.width / 2, displayResolutionSize.height - logoFace->getContentSize().height / 2 - 20);
    this->mainLayer->addChild(logoFace);
    
    float centerY = (logoFace->getPosition().y - logoFace->getContentSize().height / 2 - 30) / 2;
    
    MenuItemSprite* startGameButton = Utils::createButton((char*) TXT_START_GAME, 16, ANI_BUTTON, ANI_BUTTON);
    startGameButton->setCallback(CC_CALLBACK_1(BeginScene::onStartGameClick, this));
    startGameButton->setPosition(displayResolutionSize.width / 2, centerY + 45);
    this->nodes->setObject(startGameButton, NODE_BUTTON_START_GAME);
    
    MenuItemSprite* optionsButton = Utils::createButton((char*) TXT_OPTIONS, 16, ANI_BUTTON, ANI_BUTTON);
    optionsButton->setCallback(CC_CALLBACK_1(BeginScene::onOptionsClick, this));
    optionsButton->setPosition(displayResolutionSize.width / 2, centerY);
    this->nodes->setObject(optionsButton, NODE_BUTTON_OPTION);
    
    {
        float padding = 10;
        float posX = 0;
        float posY = centerY - 45;
        MenuItemSprite* shareFbButton = Utils::createButton(ANI_SHARE_FB_BUTTON, ANI_SHARE_FB_BUTTON);
        shareFbButton->setCallback(CC_CALLBACK_1(BeginScene::onShareFbClick, this));
        posX = posX + shareFbButton->getContentSize().width / 2 + padding;
        shareFbButton->setPosition(posX, posY);
        posX = posX + shareFbButton->getContentSize().width / 2;
        this->nodes->setObject(shareFbButton, NODE_BUTTON_FB);
        
        MenuItemSprite* shareTwButton = Utils::createButton(ANI_SHARE_TW_BUTTON, ANI_SHARE_TW_BUTTON);
        shareTwButton->setCallback(CC_CALLBACK_1(BeginScene::onShareTwClick, this));
        posX = posX + shareTwButton->getContentSize().width / 2 + padding;
        shareTwButton->setPosition(posX, posY);
        posX = posX + shareTwButton->getContentSize().width / 2;
        this->nodes->setObject(shareTwButton, NODE_BUTTON_TW);
        
        MenuItemSprite* rateButton = Utils::createButton(ANI_RATE_BUTTON, ANI_RATE_BUTTON);
        rateButton->setCallback(CC_CALLBACK_1(BeginScene::onRateClick, this));
        posX = posX + rateButton->getContentSize().width / 2 + padding;
        rateButton->setPosition(posX, posY);
        posX = posX + rateButton->getContentSize().width / 2 + padding;
        this->nodes->setObject(rateButton, NODE_BUTTON_RATE);
        
        this->mainMenu->addChild(shareFbButton);
        this->mainMenu->addChild(shareTwButton);
        this->mainMenu->addChild(rateButton);
    }
    {
        float padding = 20;
        float posX = displayResolutionSize.width;
        float posY = centerY - 45;
        MenuItemSprite* removeAdsButton = Utils::createButton(ANI_REMOVE_ADS_BUTTON, ANI_REMOVE_ADS_BUTTON);
        removeAdsButton->setCallback(CC_CALLBACK_1(BeginScene::onRemoveAdsClick, this));
        posX = posX - removeAdsButton->getContentSize().width / 2 - padding;
        removeAdsButton->setPosition(posX, posY);
        posX = posX - removeAdsButton->getContentSize().width / 2;
        this->nodes->setObject(removeAdsButton, NODE_BUTTON_REMOVE_AD);
        
        MenuItemSprite* restoreAdsButton = Utils::createButton(ANI_RESTORE_ADS_BUTTON, ANI_RESTORE_ADS_BUTTON);
        restoreAdsButton->setCallback(CC_CALLBACK_1(BeginScene::onRestoreAdsClick, this));
        posX = posX - restoreAdsButton->getContentSize().width / 2 - padding;
        restoreAdsButton->setPosition(posX, posY);
        posX = posX - restoreAdsButton->getContentSize().width / 2;
        this->nodes->setObject(restoreAdsButton, NODE_BUTTON_RESTORE_AD);
        
        this->mainMenu->addChild(removeAdsButton);
        this->mainMenu->addChild(restoreAdsButton);
    }
    
    this->mainMenu->addChild(startGameButton);
    this->mainMenu->addChild(optionsButton);
    
    // add to parent of option layer
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_START_GAME));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_OPTION));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_FB));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_TW));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RATE));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_REMOVE_AD));
    this->optionLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RESTORE_AD));
    
    // add to parent of confirm layer
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_START_GAME));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_OPTION));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_FB));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_TW));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RATE));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_REMOVE_AD));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RESTORE_AD));
}

void BeginScene::initialize() {
    this->createLayers();
    this->createLayersContent();
    
    this->listener = EventListenerKeyboard::create();
    this->listener->onKeyPressed = CC_CALLBACK_2(BeginScene::onKeyPress, this);
    this->listener->onKeyReleased = CC_CALLBACK_2(BeginScene::onKeyRelease, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void BeginScene::processEvent(GameEvent* gameEvent) {
}

void BeginScene::onStartGameClick(Ref* sender) {
    this->_eventDispatcher->removeEventListener(this->listener);
    
    MenuItemSprite* button = (MenuItemSprite*) sender;
    button->setEnabled(false);
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_START_MAP);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::onOptionsClick(Ref* sender) {
    this->optionLayer->show();
    this->handleFlashButton(sender);
}

void BeginScene::onShareFbClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_SHARE_FB);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::onShareTwClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_SHARE_TW);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::onRateClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_RATE);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::onRemoveAdsClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_REMOVE_ADS);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::onRestoreAdsClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_RESTORE_ADS);
    this->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void BeginScene::update(float dt) {
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_START_GAME));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_OPTION));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_FB));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_TW));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RATE));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_REMOVE_AD));
    this->confirmLayer->getParentButtons()->pushBack((MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RESTORE_AD));
    
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_START_GAME);
        this->checkFlash(button);
    }
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_OPTION);
        this->checkFlash(button);
    }
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_FB);
        this->checkFlash(button);
    }
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_TW);
        this->checkFlash(button);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        button->setVisible(false);
        button->setEnabled(false);
#endif
    }
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RATE);
        this->checkFlash(button);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        button->setVisible(false);
        button->setEnabled(false);
#endif
    }
    
    bool isDisplayRemoveAds = false;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    string isRemovedAd = CCApplication::sharedApplication()->getURL(":isRemovedAds:");
//    if (isRemovedAd == "1") {
//        isDisplayRemoveAds = false;
//    } else {
//        isDisplayRemoveAds = true;
//    }
//#endif
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    isDisplayRemoveAds = !AppUtils::isRemovedAds();
//#endif
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_REMOVE_AD);
        this->checkFlash(button);
        
        button->setVisible(isDisplayRemoveAds);
        button->setEnabled(isDisplayRemoveAds);
    }
    {
        MenuItemSprite* button = (MenuItemSprite*) this->nodes->objectForKey(NODE_BUTTON_RESTORE_AD);
        this->checkFlash(button);
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//        isDisplayRemoveAds = false;
//#endif
        button->setVisible(isDisplayRemoveAds);
        button->setEnabled(isDisplayRemoveAds);
    }
}

void BeginScene::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
}

void BeginScene::onKeyRelease(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
        if (!this->optionLayer->isVisible()) {
            this->confirmLayer->show();
        } else {
            this->optionLayer->hide();
        }
    }
}