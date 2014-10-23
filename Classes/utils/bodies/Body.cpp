#include "Body.h"

Body::Body() {
    this->autorelease();
    this->initialize();
}

Body::~Body() {    
}

void Body::initialize() {
    this->position = Point(0, 0);
    this->physical = true;
    this->rotation = 0;
    this->velocity = Point(0, 0);
    this->rotationVelocity = 0;
    this->scaleX = 1;
    this->scaleY = 1;
    this->priority = 0;
    this->visible = true;
    this->anchorPoint = Point(0.5f, 0.5f);
    this->flipX = false;
	this->rotationSkewY = 0;
    this->freeze = false;
}

void Body::setScale(float scale) {
    this->scaleX = scale;
    this->scaleY = scale;
}

float Body::getScale() {
    return this->scaleX;
}

Point Body::getNextPosition(float dt) {
    Point s = this->velocity * dt;
    Point nextPosition = this->position + s;
    return nextPosition;
}

float Body::getNextRotation(float dt) {
    float r = this->rotationVelocity * dt;
    float nextRotaion = this->rotation + r;
    return nextRotaion;
}

void Body::update(float dt) {
    if (!this->freeze) {
        this->position = this->getNextPosition(dt);
    }
    this->rotation = this->getNextRotation(dt);
}

bool Body::contains(Point point) {
    return false;
}

bool Body::nearby(Point point) {
    return false;
}