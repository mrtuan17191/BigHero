#include "AbstractScene.h"

AbstractScene::AbstractScene() {
    this->autorelease();
    this->nodes = __Dictionary::create();
    CC_SAFE_RETAIN(this->nodes);
}

AbstractScene::~AbstractScene() {
    CC_SAFE_RELEASE(this->nodes);
}

void AbstractScene::onEnter() {
    Scene::onEnter();
}

void AbstractScene::onExit() {
    Scene::onExit();
}

void AbstractScene::checkFlash(MenuItemSprite* button) {
    Sprite* normalImage = (Sprite*) button->getNormalImage();
    Sprite* selectedImage = (Sprite*) button->getSelectedImage();
    
    RectBody* buttonBody = new RectBody(button->getContentSize().width, button->getContentSize().height);
    buttonBody->setPosition(button->getPosition());
    
    if (button->isSelected()) {
        this->flashOn(normalImage);
        this->flashOn(selectedImage);
    } else {
        this->flashOff(normalImage);
        this->flashOff(selectedImage);
    }
}

void AbstractScene::flash(Sprite* sprite) {
    this->flashOn(sprite);
    
    FiniteTimeAction* delayAction = DelayTime::create(0.1);
    FiniteTimeAction* flashCallbackAction = CallFuncN::create(CC_CALLBACK_1(AbstractScene::flashCallback, this));
    FiniteTimeAction* sequenceAction = Sequence::create(delayAction, flashCallbackAction, NULL);
    
    sprite->runAction(sequenceAction);
}

void AbstractScene::flashCallback(Ref* sender) {
    this->flashOff((Sprite*) sender);
}

void AbstractScene::handleFlashButton(Ref* sender) {
    MenuItemSprite* button = (MenuItemSprite*) sender;
    Sprite* normalImage = (Sprite*) button->getNormalImage();
    Sprite* selectedImage = (Sprite*) button->getSelectedImage();
    
    this->flash(normalImage);
    this->flash(selectedImage);
}

void AbstractScene::flashOn(Sprite* sprite) {
    UGLayerColor* flashWhite = UGLayerColor::create();
    flashWhite->initWithColor(Color4B(255, 255, 255, 100));
    flashWhite->setContentSize(sprite->getContentSize() - Size(6, 4));
    flashWhite->setPosition(Point(4, 4));
    flashWhite->setTag(999);
    
    bool isExisted = false;
    for (int i = 0; i < sprite->getChildren().size(); i++) {
        Node* node = sprite->getChildren().at(i);
        if (node->getTag() == 999) {
            isExisted = true;
            break;
        }
    }
    
    if (!isExisted) {
        sprite->addChild(flashWhite);
    }
}

void AbstractScene::flashOff(Sprite* sprite) {
    for (int i = 0; i < sprite->getChildren().size(); i++) {
        Node* node = sprite->getChildren().at(i);
        if (node->getTag() == 999) {
            sprite->removeChildByTag(999);
            break;
        }
    }
}
