#include "PadLayer.h"

PadLayer::PadLayer(View* parent) {
    this->initialize();
    this->touchPadA = NULL;
    this->touchPadB = NULL;
    this->parent = parent;
}

PadLayer::~PadLayer() {
}

void PadLayer::initialize() {
    this->createLayers();
    this->createLayersContent();
    
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan = CC_CALLBACK_2(PadLayer::onTouchBegan, this);
    this->touchListener->onTouchMoved = CC_CALLBACK_2(PadLayer::onTouchMoved, this);
    this->touchListener->onTouchEnded = CC_CALLBACK_2(PadLayer::onTouchEnded, this);
    this->touchListener->onTouchCancelled = CC_CALLBACK_2(PadLayer::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
}

void PadLayer::createLayers() {
}

void PadLayer::createLayersContent() {
    GameModel* gameModel = GameModel::getInstance();
    Size displayResolutionSize = gameModel->getDisplayResolutionSize();
    
    this->padA = new Face();
    this->padA->initWithFile(ANI_BUTTON_A);
    this->padA->setPosition(Point(displayResolutionSize.width - CONF_PAD_PADDING_X, CONF_PAD_PADDING_Y) +
                                           Point(CONF_RANGE_TO_TOUCH, 0));
    this->addChild(this->padA);
    this->padA->setOpacity(100);
    
    this->padB = new Face();
    this->padB->initWithFile(ANI_BUTTON_B);
    this->padB->setPosition(Point(displayResolutionSize.width - CONF_PAD_PADDING_X, CONF_PAD_PADDING_Y) +
                      Point(-CONF_RANGE_TO_TOUCH, 0));
    this->addChild(padB);
    this->padB->setOpacity(100);
}

bool PadLayer::onTouchBegan(Touch* touch, Event* event) {
    Point point = this->convertTouchToNodeSpace(touch);
    if (Utils::isPointInRectangle(point, this->padA->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
        
        this->padA->setOpacity(255);
        this->touchPadA = touch;
        
        GameEvent* gameEvent = new GameEvent();
        gameEvent->setEventCode(EVT_JUMP);
        parent->fireEvent(gameEvent);
    } else if (Utils::isPointInRectangle(point, this->padB->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
        
        this->padB->setOpacity(255);
        this->touchPadB = touch;
        
        GameEvent* gameEvent = new GameEvent();
        gameEvent->setEventCode(EVT_PUNCH);
        parent->fireEvent(gameEvent);
    }
    return true;
}

void PadLayer::onTouchMoved(Touch* touch, Event* event) {
    Point point = this->convertTouchToNodeSpace(touch);
    if (touch == this->touchPadA) {
        if (Utils::isPointInRectangle(point, this->padA->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
            
            this->padA->setOpacity(255);
        } else {
            this->padA->setOpacity(100);
        }
    } else if (touch == this->touchPadB) {
        if (Utils::isPointInRectangle(point, this->padB->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
            
            this->padB->setOpacity(255);
        } else {
            this->padB->setOpacity(100);
        }
    }
}

void PadLayer::onTouchEnded(Touch* touch, Event* event) {
    Point point = this->convertTouchToNodeSpace(touch);
    if (touch == this->touchPadA) {
        if (Utils::isPointInRectangle(point, this->padA->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
            
            this->padA->setOpacity(100);
            this->touchPadA = NULL;
        }
    } else if (touch == this->touchPadB) {
        if (Utils::isPointInRectangle(point, this->padB->getPosition(), CONF_RANGE_TO_TOUCH * 2, CONF_RANGE_TO_TOUCH * 2)) {
            
            this->padB->setOpacity(100);
            this->touchPadB = NULL;
        }
    }
}

void PadLayer::onTouchCancelled(Touch* touch, Event* event) {
    this->onTouchEnded(touch, event);
}