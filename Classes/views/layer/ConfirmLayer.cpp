#include "ConfirmLayer.h"

ConfirmLayer::ConfirmLayer(View* parent) {
    this->parent = parent;
    this->initialize();
    this->parentButtons = new Vector<MenuItemSprite*>();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, SCHEDULE_LAYER, false);
}

ConfirmLayer::~ConfirmLayer() {
    CC_SAFE_DELETE(this->parentButtons);
}

void ConfirmLayer::onEnter() {
    AbstractLayer::onEnter();
}

void ConfirmLayer::onExit() {
    AbstractLayer::onExit();
    this->removeAllChildren();
    Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

void ConfirmLayer::initialize() {
    this->createLayers();
    this->createLayersContent();
}

void ConfirmLayer::createLayers() {
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
}

void ConfirmLayer::createLayersContent() {
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
    menuFace->initWithSpriteFrameName(ANI_BOX_CONFIRM);
    menuFace->setPosition(Point(gameModel->getDisplayResolutionSize().width / 2, -500));
    this->mainLayer->addChild(menuFace);
    this->menuFace = menuFace;
    Size menuFaceSize = menuFace->getContentSize();
    
    UGMenu* optionMenu = UGMenu::create();
    optionMenu->setPosition(Point::ZERO);
    menuFace->addChild(optionMenu);
    {
        Label* confirmLabel = Label::createWithBMFont(FONT_GAME_SMALL, TXT_CONFIRM_TEXT);
        confirmLabel->setPosition(menuFaceSize.width / 2, menuFaceSize.height / 2);
        this->menuFace->addChild(confirmLabel);
    }
    {
        FiniteTimeAction* move1 = MoveTo::create(0, menuFace->getPosition());
        FiniteTimeAction* move2 = MoveTo::create(0.5f, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move3 = CallFunc::create(CC_CALLBACK_0(ConfirmLayer::onOptionBoardMoveUpDone, this));
        FiniteTimeAction* move123 = Sequence::create(move1, move2, move3, NULL);
        menuFace->registerState(STATE_MOVE_UP, move123);
        
        FiniteTimeAction* move4 = MoveTo::create(0, Point(gameModel->getDisplayResolutionSize().width / 2, gameModel->getDisplayResolutionSize().height / 2));
        FiniteTimeAction* move5 = MoveTo::create(0.5f, menuFace->getPosition());
        FiniteTimeAction* move6 = CallFunc::create(CC_CALLBACK_0(ConfirmLayer::onOptionBoardMoveDownDone, this));
        FiniteTimeAction* move456 = Sequence::create(move4, move5, move6, NULL);
        menuFace->registerState(STATE_MOVE_DOWN, move456);
        
        MenuItemSprite* okButton = Utils::createButton((char*) TXT_OK, 16, ANI_BUTTON_SMALL, ANI_BUTTON_SMALL);
        okButton->setCallback(CC_CALLBACK_1(ConfirmLayer::onOkButtonClick, this));
        okButton->setPosition(Point(menuFaceSize.width / 2 - 129, 0));
        okButton->setEnabled(false);
        optionMenu->addChild(okButton);
        this->okButton = okButton;
        
        MenuItemSprite* cancelButton = Utils::createButton((char*) TXT_CANCEL, 16, ANI_BUTTON_SMALL, ANI_BUTTON_SMALL);
        cancelButton->setCallback(CC_CALLBACK_1(ConfirmLayer::onCancelButtonClick, this));
        cancelButton->setPosition(Point(menuFaceSize.width / 2 + 129, 0));
        cancelButton->setEnabled(false);
        optionMenu->addChild(cancelButton);
        this->cancelButton = cancelButton;
    }
}

void ConfirmLayer::enableParentButton(bool enabled) {
    for (int i = 0; i < this->parentButtons->size(); i++) {
        this->parentButtons->at(i)->setEnabled(enabled);
    }
}

void ConfirmLayer::enableButton(bool enabled) {
    this->okButton->setEnabled(enabled);
    this->cancelButton->setEnabled(enabled);
}

void ConfirmLayer::show() {
    this->setVisible(true);
    this->enableParentButton(false);
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_IN));
    this->menuFace->setState(menuFace->getState(STATE_MOVE_UP));
}

void ConfirmLayer::onOptionBoardMoveUpDone() {
    this->enableButton(true);
}

void ConfirmLayer::onOptionBoardMoveDownDone() {
    this->setVisible(false);
    this->enableParentButton(true);
}

void ConfirmLayer::onOkButtonClick(Ref* sender) {
    CCDirector::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void ConfirmLayer::hide() {
    this->enableButton(false);
    
    this->backgroundFade->setState(this->backgroundFade->getState(STATE_FADE_OUT));
    this->menuFace->setState(menuFace->getState(STATE_MOVE_DOWN));
}

void ConfirmLayer::onCancelButtonClick(Ref* sender) {
    this->hide();
}

void ConfirmLayer::update(float dt) {
    this->checkFlash(this->okButton);
    this->checkFlash(this->cancelButton);
}