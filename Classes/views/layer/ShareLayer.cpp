#include "ShareLayer.h"

ShareLayer::ShareLayer(View* parent) {
    this->parent = parent;
    this->initialize();
    this->parentButtons = new Vector<MenuItemSprite*>();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, SCHEDULE_LAYER, false);
}

ShareLayer::~ShareLayer() {
    CC_SAFE_DELETE(this->parentButtons);
}

void ShareLayer::onEnter() {
    AbstractLayer::onEnter();
}

void ShareLayer::onExit() {
    AbstractLayer::onExit();
    this->removeAllChildren();
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void ShareLayer::initialize() {
    this->createLayers();
    this->createLayersContent();
}

void ShareLayer::createLayers() {
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
}

void ShareLayer::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    
    Face* backgroundFade = new Face();
    backgroundFade->initWithFile(ANI_BACKGROUND_FADE);
    backgroundFade->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
    backgroundFade->setOpacity(0);
    this->mainLayer->addChild(backgroundFade);
    this->backgroundFade = backgroundFade;
    this->nodes->setObject(backgroundFade, NODE_FADE_BG);
    {
        FiniteTimeAction* fadeIn = FadeIn::create(0.5f);
        backgroundFade->registerState(STATE_FADE_IN, fadeIn);
        
        FiniteTimeAction* fadeOut = FadeOut::create(0.5f);
        backgroundFade->registerState(STATE_FADE_OUT, fadeOut);
    }
    
    // board
    Face* menuFace = new Face();
    menuFace->initWithSpriteFrameName(ANI_BOX_SHARE);
    menuFace->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height + 500));
    this->mainLayer->addChild(menuFace);
    this->menuFace = menuFace;
    this->nodes->setObject(menuFace, NODE_SHARE_BOARD);
    Size menuFaceSize = menuFace->getContentSize();
    {
        Label* requireShareLabel = Label::createWithBMFont(FONT_GAME_SMALL, "");
        this->nodes->setObject(requireShareLabel, NODE_LABEL_SHARE);
        menuFace->addChild(requireShareLabel);
    }
    
    UGMenu* pauseMenu = UGMenu::create();
    pauseMenu->setPosition(Point::ZERO);
    menuFace->addChild(pauseMenu);
    
    {
        FiniteTimeAction* move1 = MoveTo::create(0, menuFace->getPosition());
        FiniteTimeAction* move2 = MoveTo::create(0.5f, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move3 = CallFunc::create(CC_CALLBACK_0(ShareLayer::onShareBoardMoveDownDone, this));
        FiniteTimeAction* move123 = Sequence::create(move1, move2, move3, NULL);
        menuFace->registerState(STATE_MOVE_DOWN, move123);
        
        FiniteTimeAction* move4 = MoveTo::create(0, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move5 = MoveTo::create(0.5f, menuFace->getPosition());
        FiniteTimeAction* move6 = CallFunc::create(CC_CALLBACK_0(ShareLayer::onShareBoardMoveUpDone, this));
        FiniteTimeAction* move456 = Sequence::create(move4, move5, move6, NULL);
        menuFace->registerState(STATE_MOVE_UP, move456);
        
        MenuItemSprite* facebookButton = Utils::createButton((char*) TXT_SHARE_FACEBOOK, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
        facebookButton->setCallback(CC_CALLBACK_1(ShareLayer::onShareFacebookClick, this));
        facebookButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25 + 50));
        facebookButton->setEnabled(false);
        pauseMenu->addChild(facebookButton);
        this->shareFacebookButton = facebookButton;
        this->nodes->setObject(facebookButton, NODE_BUTTON_FACEBOOK);
        
        MenuItemSprite* twitterButton = Utils::createButton((char*) TXT_SHARE_TWITTER, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
        twitterButton->setCallback(CC_CALLBACK_1(ShareLayer::onShareTwitterClick, this));
        twitterButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25));
        twitterButton->setEnabled(false);
        pauseMenu->addChild(twitterButton);
        this->shareTwitterButton = twitterButton;
        this->nodes->setObject(twitterButton, NODE_BUTTON_TWITTER);
        
        MenuItemSprite* okButton = Utils::createButton((char*) TXT_SHARE_OK, 32, ANI_PAUSE_BUTTON, ANI_PAUSE_BUTTON);
        okButton->setCallback(CC_CALLBACK_1(ShareLayer::onOkButtonClick, this));
        okButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 25 - 50));
        okButton->setEnabled(false);
        pauseMenu->addChild(okButton);
        this->okButton = okButton;
        this->nodes->setObject(okButton, NODE_BUTTON_OK);
    }
}

void ShareLayer::enableParentButton(bool enabled) {
    for (int i = 0; i < this->parentButtons->size(); i++) {
        this->parentButtons->at(i)->setEnabled(enabled);
    }
}

void ShareLayer::enableButton(bool enabled) {
    this->okButton->setEnabled(enabled);
    this->shareFacebookButton->setEnabled(enabled);
    this->shareTwitterButton->setEnabled(enabled);
}

void ShareLayer::show(string message) {
    this->setVisible(true);
    this->enableParentButton(false);
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_IN));
    this->menuFace->setState(this->menuFace->getState(STATE_MOVE_DOWN));
    
    Size menuFaceSize = this->menuFace->getContentSize();
    Label* requireShareLabel = (Label*) this->nodes->objectForKey(NODE_LABEL_SHARE);
    requireShareLabel->setString(message);
    requireShareLabel->setDimensions(menuFaceSize.width - 20, 0);
    requireShareLabel->setPosition(Point(20 + requireShareLabel->getDimensions().width / 2, menuFaceSize.height  - requireShareLabel->getContentSize().height / 2 - 12));
}

void ShareLayer::onShareBoardMoveUpDone() {
    this->setVisible(false);
    this->enableParentButton(true);
}

void ShareLayer::onShareBoardMoveDownDone() {
    this->enableButton(true);
}

void ShareLayer::onOkButtonClick(Ref* sender) {
    this->hide();
    this->handleFlashButton(sender);
}

void ShareLayer::onShareFacebookClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_SHARE_FB);
    this->parent->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void ShareLayer::onShareTwitterClick(Ref* sender) {
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_SHARE_TW);
    this->parent->fireEvent(gameEvent);
    this->handleFlashButton(sender);
}

void ShareLayer::hide() {
    this->enableButton(false);
    
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_OUT));
    this->menuFace->setState(menuFace->getState(STATE_MOVE_UP));
}

void ShareLayer::update(float dt) {
    this->checkFlash(this->okButton);
    this->checkFlash(this->shareFacebookButton);
    this->checkFlash(this->shareTwitterButton);
}