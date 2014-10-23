#include "StatusBar.h"

StatusBar::StatusBar() {
    this->autorelease();
}

bool StatusBar::init() {
    return Sprite::init();
}

void StatusBar::initWithParameter(char* barFrameName, char* contentFrameName) {
    this->initWithFile(barFrameName);
	this->contentFace = Sprite::create(contentFrameName);
    this->contentFaceSize = this->contentFace->getContentSize();
    this->contentFace->setAnchorPoint(Point(0, 0.5f));
    
    Size slotSize = this->getContentSize();
    float posX = (slotSize.width - this->contentFace->getContentSize().width) / 2;
    float posY = slotSize.height / 2;
    this->contentFace->setPosition(posX, posY);
    
	this->addChild(this->contentFace);
	this->value = 0;
}

StatusBar::~StatusBar() {
}

float StatusBar::getValue() {
	return this->value;
}

void StatusBar::setValue(float value) {
	this->value = value;
    float currentWidth = contentFaceSize.width * value;
    this->contentFace->setTextureRect(Rect(0, 0, currentWidth, this->contentFace->getContentSize().height));
}