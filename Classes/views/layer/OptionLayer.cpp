#include "OptionLayer.h"

OptionLayer::OptionLayer(View* parent) {
    this->parent = parent;
    this->initialize();
    this->parentButtons = new Vector<MenuItemSprite*>();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, SCHEDULE_LAYER, false);
}

OptionLayer::~OptionLayer() {
    CC_SAFE_DELETE(this->parentButtons);
}

void OptionLayer::onEnter() {
    AbstractLayer::onEnter();
}

void OptionLayer::onExit() {
    AbstractLayer::onExit();
    this->removeAllChildren();
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void OptionLayer::initialize() {
    this->createLayers();
    this->createLayersContent();
}

void OptionLayer::createLayers() {
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
}

void OptionLayer::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    
    Face* backgroundFade = new Face();
    backgroundFade->initWithFile(ANI_BACKGROUND_FADE);
    backgroundFade->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
    backgroundFade->setOpacity(0);
    this->mainLayer->addChild(backgroundFade);
    this->backgroundFade = backgroundFade;
    {
        FiniteTimeAction* fadeIn = FadeIn::create(0.5f);
        backgroundFade->registerState(STATE_FADE_IN, fadeIn);
        
        FiniteTimeAction* fadeOut = FadeOut::create(0.5f);
        backgroundFade->registerState(STATE_FADE_OUT, fadeOut);
    }
    
    Face* menuFace = new Face();
    menuFace->initWithSpriteFrameName(ANI_BOX_OPTION);
    menuFace->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, -500));
    this->mainLayer->addChild(menuFace);
    this->menuFace = menuFace;
    Size menuFaceSize = menuFace->getContentSize();
    
    UGMenu* optionMenu = UGMenu::create();
    optionMenu->setPosition(Point::ZERO);
    menuFace->addChild(optionMenu);
    
    {
        FiniteTimeAction* move1 = MoveTo::create(0, menuFace->getPosition());
        FiniteTimeAction* move2 = MoveTo::create(0.5f, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move3 = CallFunc::create(CC_CALLBACK_0(OptionLayer::onOptionBoardMoveUpDone, this));
        FiniteTimeAction* move123 = Sequence::create(move1, move2, move3, NULL);
        menuFace->registerState(STATE_MOVE_UP, move123);
        
        FiniteTimeAction* move4 = MoveTo::create(0, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move5 = MoveTo::create(0.5f, menuFace->getPosition());
        FiniteTimeAction* move6 = CallFunc::create(CC_CALLBACK_0(OptionLayer::onOptionBoardMoveDownDone, this));
        FiniteTimeAction* move456 = Sequence::create(move4, move5, move6, NULL);
        menuFace->registerState(STATE_MOVE_DOWN, move456);
        
        char musicText[200];
        sprintf(musicText, TXT_MUSIC_PROGRESS, gameModel->getSavedData()->isMusicOn() ? "ON": "OFF");
        MenuItemSprite* musicButton = Utils::createButton(musicText, 16, ANI_BUTTON, ANI_BUTTON);
        musicButton->setCallback(CC_CALLBACK_1(OptionLayer::onMusicButtonClick, this));
        musicButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 + 10));
        musicButton->setEnabled(false);
        optionMenu->addChild(musicButton);
        this->musicButton = musicButton;
        
        char sfxText[200];
        sprintf(sfxText, TXT_SFX_PROGRESS, gameModel->getSavedData()->isSfxOn() ? "ON": "OFF");
        MenuItemSprite* sfxButton = Utils::createButton(sfxText, 16, ANI_BUTTON, ANI_BUTTON);
        sfxButton->setCallback(CC_CALLBACK_1(OptionLayer::onSfxButtonClick, this));
        sfxButton->setPosition(Point(menuFaceSize.width / 2, menuFaceSize.height / 2 - 40));
        sfxButton->setEnabled(false);
        optionMenu->addChild(sfxButton);
        this->sfxButton = sfxButton;
        
        MenuItemSprite* closeButton = Utils::createButton(ANI_CLOSE_BUTTON, ANI_CLOSE_BUTTON);
        closeButton->setCallback(CC_CALLBACK_1(OptionLayer::onCloseButtonClick, this));
        closeButton->setPosition(Point(menuFaceSize.width, menuFaceSize.height));
        closeButton->setEnabled(false);
        optionMenu->addChild(closeButton);
        this->closeButton = closeButton;
    }
}

void OptionLayer::enableParentButton(bool enabled) {
    for (int i = 0; i < this->parentButtons->size(); i++) {
        this->parentButtons->at(i)->setEnabled(enabled);
    }
}

void OptionLayer::enableButton(bool enabled) {
    this->musicButton->setEnabled(enabled);
    this->sfxButton->setEnabled(enabled);
    this->closeButton->setEnabled(enabled);
}

void OptionLayer::show() {
    this->setVisible(true);
    this->enableParentButton(false);
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_IN));
    this->menuFace->setState(menuFace->getState(STATE_MOVE_UP));
}

void OptionLayer::onOptionBoardMoveUpDone() {
    this->enableButton(true);
}

void OptionLayer::onOptionBoardMoveDownDone() {
    this->setVisible(false);
    this->enableParentButton(true);
}

void OptionLayer::onMusicButtonClick(Ref* sender) {
    GameModel* gameModel = GameModel::getInstance();
    int musicOnInt = !gameModel->getSavedData()->isMusicOn();
    DataEvent* dataEvent = new DataEvent();
    dataEvent->setEventCode(EVT_CHANGE_MUSIC);
    dataEvent->setArgumentInt(musicOnInt);
    this->parent->fireEvent(dataEvent);
    
    char musicText[200];
    sprintf(musicText, TXT_MUSIC_PROGRESS, gameModel->getSavedData()->isMusicOn() ? "ON": "OFF");
    Label* normalText = (Label*) this->musicButton->getNormalImage()->getChildren().at(0);
    Label* selectedText = (Label*) this->musicButton->getSelectedImage()->getChildren().at(0);
    normalText->setString(musicText);
    selectedText->setString(musicText);
    
    this->handleFlashButton(sender);
}

void OptionLayer::onSfxButtonClick(Ref* sender) {
    GameModel* gameModel = GameModel::getInstance();
    int sfxOnInt = !gameModel->getSavedData()->isSfxOn();
    DataEvent* dataEvent = new DataEvent();
    dataEvent->setEventCode(EVT_CHANGE_SFX);
    dataEvent->setArgumentInt(sfxOnInt);
    this->parent->fireEvent(dataEvent);
    
    char sfxText[200];
    sprintf(sfxText, TXT_SFX_PROGRESS, gameModel->getSavedData()->isSfxOn() ? "ON": "OFF");
    Label* normalText = (Label*) this->sfxButton->getNormalImage()->getChildren().at(0);
    Label* selectedText = (Label*) this->sfxButton->getSelectedImage()->getChildren().at(0);
    normalText->setString(sfxText);
    selectedText->setString(sfxText);
    
    this->handleFlashButton(sender);
}

void OptionLayer::hide() {
    this->enableButton(false);
    
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_OUT));
    this->menuFace->setState(menuFace->getState(STATE_MOVE_DOWN));
}

void OptionLayer::onCloseButtonClick(Ref* sender) {
    this->hide();
}

void OptionLayer::update(float dt) {
    this->checkFlash(this->musicButton);
    this->checkFlash(this->sfxButton);
}