#include "GalleryScene.h"

GalleryScene::GalleryScene() {
    this->currentCharacterIndex = 0;
    this->currentAnimationIndex = 0;
    this->totalCharacters = __Array::create();
    CC_SAFE_RETAIN(this->totalCharacters);
    this->totalAnimations = __Array::create();
    CC_SAFE_RETAIN(this->totalAnimations);
    this->requestRefresh = false;
    this->requestRefreshAnimation = false;
    this->currentCharacter = NULL;
    this->initialize();
}

GalleryScene::~GalleryScene() {
    Utils::safeRelease(this->totalCharacters);
    Utils::safeRelease(this->totalAnimations);
    Utils::safeRelease(this->currentCharacter);
}

void GalleryScene::onEnter() {
    AbstractScene::onEnter();
}

void GalleryScene::onExit() {
    AbstractScene::onExit();
    this->removeAllChildren();
}

void GalleryScene::createLayers() {
    UGLayerColor* colorLayer = UGLayerColor::create();
    colorLayer->initWithColor(Color4B(255, 255, 255, 255));
    this->addChild(colorLayer);
    
    this->backgroundLayer = UGLayer::create();
    this->addChild(this->backgroundLayer);
    
    this->mainLayer = UGLayer::create();
    this->addChild(this->mainLayer);
}

void GalleryScene::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    this->totalCharacters->addObject(__String::create(KEY_AGENT));
    this->totalCharacters->addObject(__String::create(KEY_AXE_FIREMAN));
    this->totalCharacters->addObject(__String::create(KEY_BASEBALL_BATTER));
    this->totalCharacters->addObject(__String::create(KEY_BASEBALL_PITCHER));
    this->totalCharacters->addObject(__String::create(KEY_BATON_COP));
    this->totalCharacters->addObject(__String::create(KEY_BIKER));
    this->totalCharacters->addObject(__String::create(KEY_BOSS1));
    this->totalCharacters->addObject(__String::create(KEY_BOSS2));
    this->totalCharacters->addObject(__String::create(KEY_BOSS3));
    this->totalCharacters->addObject(__String::create(KEY_BOSS4));
    this->totalCharacters->addObject(__String::create(KEY_BOSS5));
    this->totalCharacters->addObject(__String::create(KEY_CODER));
    this->totalCharacters->addObject(__String::create(KEY_DOORS));
    this->totalCharacters->addObject(__String::create(KEY_FIST_COP));
    this->totalCharacters->addObject(__String::create(KEY_FIST_FIREMAN));
    this->totalCharacters->addObject(__String::create(KEY_FIST_SOLDIER));
    this->totalCharacters->addObject(__String::create(KEY_FORKLIFT));
    this->totalCharacters->addObject(__String::create(KEY_FX));
    this->totalCharacters->addObject(__String::create(KEY_GOLFER));
    this->totalCharacters->addObject(__String::create(KEY_GORILLA));
    this->totalCharacters->addObject(__String::create(KEY_GUARD_BIG));
    this->totalCharacters->addObject(__String::create(KEY_GUARD));
    this->totalCharacters->addObject(__String::create(KEY_LASER_NERD));
    this->totalCharacters->addObject(__String::create(KEY_MANLY_MAN));
    this->totalCharacters->addObject(__String::create(KEY_MIME));
    this->totalCharacters->addObject(__String::create(KEY_NINJA));
    this->totalCharacters->addObject(__String::create(KEY_PISTOL_COP));
    this->totalCharacters->addObject(__String::create(KEY_PLAYER));
    this->totalCharacters->addObject(__String::create(KEY_PROJECTILES));
    this->totalCharacters->addObject(__String::create(KEY_RIFLE_SOLDIER));
    this->totalCharacters->addObject(__String::create(KEY_RIOT_COP));
    this->totalCharacters->addObject(__String::create(KEY_RUNNER));
    this->totalCharacters->addObject(__String::create(KEY_SOLDIER_BIG));
    this->totalCharacters->addObject(__String::create(KEY_SPIKES));
    this->totalCharacters->addObject(__String::create(KEY_TANK));
    this->totalCharacters->addObject(__String::create(KEY_WATER_SPRAY));
    this->totalCharacters->addObject(__String::create(KEY_WEAPONS));
    this->totalCharacters->addObject(__String::create(KEY_WORKER));
    this->totalCharacters->addObject(__String::create(KEY_ZOMBIE));
    
    string key = ((__String*) this->totalCharacters->getObjectAtIndex(this->currentCharacterIndex))->getCString();
    this->currentCharacter = new Man();
    CC_SAFE_RETAIN(this->currentCharacter);
    this->currentCharacter->initFaces(key, gameModel->getAnimations());
    this->currentCharacter->setPosition(Point(displayResolutionSize.width / 2, displayResolutionSize.height / 2));
    this->addFacesToLayer(this->currentCharacter, this->mainLayer, 1);

    char stateStr[200];
    string state = this->currentCharacter->getListStates()->at(this->currentAnimationIndex)->getCString();
    sprintf(stateStr, "%s", state.data());
    this->currentCharacter->setState(stateStr, (char*) FACE_MAIN);
    for (int i = 0; i< this->currentCharacter->getListStates()->size(); i++) {
        __String* state = this->currentCharacter->getListStates()->at(i);
        this->totalAnimations->addObject(__String::create(state->getCString()));
    }
    
    char nameStr[200];
    sprintf(nameStr, "%s", key. data());
    this->characterLabel = Utils::createLabel(nameStr, 20);
    this->characterLabel->setDimensions(designResolutionSize.width, 0);
    this->characterLabel->setPosition(displayResolutionSize.width / 2, displayResolutionSize.height - 50);
    this->mainLayer->addChild(this->characterLabel);
    
    this->skillNameLabel = Utils::createLabel(stateStr, 20);
    this->skillNameLabel->setDimensions(designResolutionSize.width, 0);
    this->skillNameLabel->setPosition(displayResolutionSize.width / 2, 50);
    this->mainLayer->addChild(this->skillNameLabel);
    
    UGMenu* menu = UGMenu::create();
    menu->setPosition(Point::ZERO);
    this->mainLayer->addChild(menu);

    MenuItemLabel* nextCharacter = Utils::createButtonLabel((char*) "NEXT OBJ", 20);
    nextCharacter->setCallback(CC_CALLBACK_1(GalleryScene::onNextCharacterClick, this));
    nextCharacter->setPosition(displayResolutionSize.width / 2 - 100, displayResolutionSize.height / 2 + 50);
    
    MenuItemLabel* previousCharacter = Utils::createButtonLabel((char*) "PREVIOUS OBJ", 20);
    previousCharacter->setCallback(CC_CALLBACK_1(GalleryScene::onPreviousCharacterClick, this));
    previousCharacter->setPosition(displayResolutionSize.width / 2 - 100, displayResolutionSize.height / 2 - 50);
    
    MenuItemLabel* nextAnimation = Utils::createButtonLabel((char*) "NEXT STATE", 20);
    nextAnimation->setCallback(CC_CALLBACK_1(GalleryScene::onNextAnimationClick, this));
    nextAnimation->setPosition(displayResolutionSize.width / 2 + 100, displayResolutionSize.height / 2 + 50);
    
    MenuItemLabel* previousAnimation = Utils::createButtonLabel((char*) "PREVIOUS STATE", 20);
    previousAnimation->setCallback(CC_CALLBACK_1(GalleryScene::onPreviousAnimationClick, this));
    previousAnimation->setPosition(displayResolutionSize.width / 2 + 100, displayResolutionSize.height / 2 - 50);
    
    menu->addChild(nextCharacter);
    menu->addChild(previousCharacter);
    menu->addChild(nextAnimation);
    menu->addChild(previousAnimation);
}

void GalleryScene::initialize() {
    this->createLayers();
    this->createLayersContent();
        
    this->touchListener = EventListenerTouchAllAtOnce::create();
    this->touchListener->onTouchesBegan = CC_CALLBACK_2(GalleryScene::onTouchesBegan, this);
    this->touchListener->onTouchesMoved = CC_CALLBACK_2(GalleryScene::onTouchesMoved, this);
    this->touchListener->onTouchesEnded = CC_CALLBACK_2(GalleryScene::onTouchesEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
    
    this->listener = EventListenerKeyboard::create();
    this->listener->onKeyPressed = CC_CALLBACK_2(GalleryScene::onKeyPress, this);
    this->listener->onKeyReleased = CC_CALLBACK_2(GalleryScene::onKeyRelease, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listener, this);
}

void GalleryScene::addFacesToLayer(Obj* object, Layer* layer, int index) {
    for (int i = 0; i < object->getFaces()->count(); i++) {
        Face* node = (Face*) object->getFaces()->getObjectAtIndex(i);
        node->setPosition(object->getPosition());
        node->setAnchorPoint(object->getBody()->getAnchorPoint());
        node->setFlippedX(object->getBody()->isFlipX());
        node->setScale(object->getBody()->getScale());
        layer->addChild(node, index);
    }
}

void GalleryScene::removeFaces(Obj* object) {
    for (int i = 0; i < object->getFaces()->count(); i++) {
        Node* node = (Node*) object->getFaces()->getObjectAtIndex(i);
        node->removeFromParentAndCleanup(true);
    }
}

void GalleryScene::update(float dt) {
    if (this->requestRefresh) {
        this->requestRefresh = false;
        
        this->removeFaces(this->currentCharacter);
        CC_SAFE_RELEASE(this->currentCharacter);
        this->totalAnimations->removeAllObjects();
        this->currentAnimationIndex = 0;
        
        GameModel* gameModel = GameModel::getInstance();
        Size displayResolutionSize = gameModel->getDisplayResolutionSize();
        
        string key = ((__String*) this->totalCharacters->getObjectAtIndex(this->currentCharacterIndex))->getCString();
        this->currentCharacter = new Man();
        CC_SAFE_RETAIN(this->currentCharacter);
        this->currentCharacter->initFaces(key, gameModel->getAnimations());
        this->currentCharacter->setPosition(Point(displayResolutionSize.width / 2, displayResolutionSize.height / 2));
        this->addFacesToLayer(this->currentCharacter, this->mainLayer, 1);
        
        char stateStr[200];
        string state = this->currentCharacter->getListStates()->at(this->currentAnimationIndex)->getCString();
        sprintf(stateStr, "%s", state.data());
        this->currentCharacter->setState(stateStr, (char*) FACE_MAIN);
        for (int i = 0; i< this->currentCharacter->getListStates()->size(); i++) {
            __String* state = this->currentCharacter->getListStates()->at(i);
            this->totalAnimations->addObject(__String::create(state->getCString()));
        }
        
        char nameStr[200];
        sprintf(nameStr, "%s", key.data());
        this->characterLabel->setString(nameStr);
        
        this->skillNameLabel->setString(stateStr);
    }
    
    if (this->requestRefreshAnimation) {
        this->requestRefreshAnimation = false;
        
        char stateStr[200];
        string state = ((__String*) this->totalAnimations->getObjectAtIndex(this->currentAnimationIndex))->getCString();
        sprintf(stateStr, "%s", state.data());
        this->currentCharacter->setState(stateStr, (char*) FACE_MAIN);
        this->skillNameLabel->setString(stateStr);
    }
}

void GalleryScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* event) {
}

void GalleryScene::onTouchesMoved(const std::vector<Touch*>& touches, Event* event) {
}

void GalleryScene::onTouchesEnded(const std::vector<Touch*>& touches, Event* event) {
}

void GalleryScene::onTouchesCancelled(const std::vector<Touch*>& touches, Event* event) {
    this->onTouchesEnded(touches, event);
}

void GalleryScene::onKeyPress(EventKeyboard::KeyCode keyCode, Event* event) {
}

void GalleryScene::onKeyRelease(EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
    }
}

void GalleryScene::onNextCharacterClick(Ref* sender) {
    int totalCharacter = this->totalCharacters->count();
    int newCharacterIndex = this->currentCharacterIndex + 1;
    if (newCharacterIndex >= totalCharacter) {
        this->currentCharacterIndex = 0;
    } else {
        this->currentCharacterIndex = newCharacterIndex;
    }
    this->requestRefresh = true;
    CCLOG("Character: %d", this->currentCharacterIndex);
}

void GalleryScene::onPreviousCharacterClick(Ref* sender) {
    int totalCharacter = this->totalCharacters->count();
    int newCharacterIndex = this->currentCharacterIndex - 1;
    if (newCharacterIndex < 0) {
        this->currentCharacterIndex = totalCharacter - 1;
    } else {
        this->currentCharacterIndex = newCharacterIndex;
    }
    this->requestRefresh = true;
    CCLOG("Character: %d", this->currentCharacterIndex);
}

void GalleryScene::onNextAnimationClick(Ref* sender) {
    int totalAnimation = this->totalAnimations->count();
    int newAnimationIndex = this->currentAnimationIndex + 1;
    if (newAnimationIndex >= totalAnimation) {
        this->currentAnimationIndex = 0;
    } else {
        this->currentAnimationIndex = newAnimationIndex;
    }
    this->requestRefreshAnimation = true;
    CCLOG("Animation: %d", this->currentAnimationIndex);
}

void GalleryScene::onPreviousAnimationClick(Ref* sender) {
    int totalAnimation = this->totalAnimations->count();
    int newAnimationIndex = this->currentAnimationIndex - 1;
    if (newAnimationIndex < 0) {
        this->currentAnimationIndex = totalAnimation - 1;
    } else {
        this->currentAnimationIndex = newAnimationIndex;
    }
    this->requestRefreshAnimation = true;
    CCLOG("Animation: %d", this->currentAnimationIndex);
}

