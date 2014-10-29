#include "ControllerLayer.h"

ControllerLayer::ControllerLayer(View* parent) {
    this->parent = parent;
    this->isPressed = false;
    this->kCenter = Point(CONF_JOYSTICK_RADIUS + CONF_JOYSTICK_OFFSET_X,
                          CONF_JOYSTICK_RADIUS + CONF_JOYSTICK_OFFSET_Y);
    this->velocity = Point::ZERO;
    this->initialize();
    this->setVisible(false);
}

ControllerLayer::~ControllerLayer() {
}

void ControllerLayer::initialize() {
    this->createLayers();
    this->createLayersContent();
    
    this->touchListener = EventListenerTouchOneByOne::create();
    this->touchListener->onTouchBegan = CC_CALLBACK_2(ControllerLayer::onTouchBegan, this);
    this->touchListener->onTouchMoved = CC_CALLBACK_2(ControllerLayer::onTouchMoved, this);
    this->touchListener->onTouchEnded = CC_CALLBACK_2(ControllerLayer::onTouchEnded, this);
    this->touchListener->onTouchCancelled = CC_CALLBACK_2(ControllerLayer::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
}

void ControllerLayer::createLayers() {
//    UGLayerColor* layerColoer = UGLayerColor::create();
//    layerColoer->initWithColor(Color4B(0, 0, 0, 100));
//    this->addChild(layerColoer);
    /* background */
    Face* bg = new Face();
    bg->initWithFile(ANI_JOYSTICK_BACKGROUND);
    bg->setPosition(this->kCenter);
    bg->setVisible(false);
    this->addChild(bg, 0);
    
    /* thumb */
    this->thumb = new Face();
    this->thumb->initWithFile(ANI_JOYSTICK_THUMB);
    this->thumb->setPosition(this->kCenter);
    this->thumb->setOpacity(100);
    this->addChild(this->thumb, 1);
}

void ControllerLayer::createLayersContent() {
}

bool ControllerLayer::onTouchBegan(Touch* touch, Event* event) {
    if (this->isVisible()) {
        return false;
    }
    Point point = this->convertTouchToNodeSpace(touch);
    Size contentSize = this->getParent()->getContentSize();
    Point center = this->getParent()->getPosition() + Point(contentSize.width / 2, contentSize.height / 2);
    if (Utils::isPointInRectangle(point, center, contentSize.width, contentSize.height)) {
        //new point same as point
        Point newPoint = point - this->getParent()->getPosition();
        
        this->setPosition(newPoint - this->kCenter);
        this->isPressed = true;
        this->setVisible(true);
        return true;
    }
    return false;
}

void ControllerLayer::onTouchMoved(Touch* touch, Event* event) {
    if (this->isPressed) {
        Point point = this->convertTouchToNodeSpace(touch);
        this->updateVelocity(point);
        DataEvent* dataEvent = new DataEvent();
        dataEvent->setEventCode(EVT_JOYSTICK_MOVE);
        dataEvent->setArgumentFloat(this->getAngle());
        this->parent->fireEvent(dataEvent);
    }
}

void ControllerLayer::onTouchEnded(Touch* touch, Event* event) {
    this->handleLastTouch();
	this->setVisible(false);
    
    GameEvent* gameEvent = new GameEvent();
    gameEvent->setEventCode(EVT_JOYSTICK_STAND);
    this->parent->fireEvent(gameEvent);
}

void ControllerLayer::onTouchCancelled(Touch* touch, Event* event) {
    this->onTouchEnded(touch, event);
}

void ControllerLayer::updateVelocity(Point point) {
    float dx = point.x - this->kCenter.x;
    float dy = point.y - this->kCenter.y;
    
    float distance = sqrt(dx * dx + dy * dy);
    float angle = atan2(dy, dx); // in radians
    
    if (distance > CONF_JOYSTICK_RADIUS) {
        dx = cos(angle) * CONF_JOYSTICK_RADIUS;
        dy = sin(angle) * CONF_JOYSTICK_RADIUS;
        
        float rate = CONF_JOYSTICK_RADIUS / distance;
        Point vMove = Point(dx, dy) * (1 - rate);
        this->setPosition(this->getPosition() + vMove);
    }

    this->velocity = Point(dx / CONF_JOYSTICK_RADIUS, dy / CONF_JOYSTICK_RADIUS);
    
    if (distance > CONF_THUMB_RADIUS) {
        point.x = this->kCenter.x + cos(angle) * CONF_THUMB_RADIUS;
        point.y = this->kCenter.y + sin(angle) * CONF_THUMB_RADIUS;
    }
    
    this->thumb->setPosition(point);
}

void ControllerLayer::stopJoystick() {
    this->velocity = Point::ZERO;
    this->setVisible(false);
}

Point ControllerLayer::getVelocity() {
    return this->velocity;
}

float ControllerLayer::getAngle() {
    float angle = atan2(this->velocity.y, this->velocity.x);
    float dx = cos(angle) * CONF_JOYSTICK_RADIUS;
    float dy = sin(angle) * CONF_JOYSTICK_RADIUS;
    float newAngle = atan2(dy, dx); // in radians
    return newAngle;
}

void ControllerLayer::resetJoystick() {
    this->updateVelocity(this->kCenter);
}

bool ControllerLayer::handleLastTouch() {
    bool wasPressed = this->isPressed;
    if (wasPressed) {
        this->resetJoystick();
        this->isPressed = false;
        this->setPosition(Point::ZERO);
    }
    return (wasPressed ? true : false);
}