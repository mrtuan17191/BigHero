#include "RectBody.h"

RectBody::RectBody(float width, float height) {
    this->width = width;
	this->height = height;
    this->properties = __Dictionary::create();
    CC_SAFE_RETAIN(this->properties);
    this->setRect(true);
}

RectBody::~RectBody() {
    CC_SAFE_RELEASE(this->properties);
}

bool RectBody::contains(Point point) {
    return this->containsInDistance(point, 1);
}

bool RectBody::containsInDistance(Point point, float distanceRate) {
    if (point.x >= this->position.x - this->width / 2 * distanceRate
        && point.x <= this->position.x + this->width / 2 * distanceRate
        && point.y >= this->position.y - this->height / 2 * distanceRate
        && point.y <= this->position.y + this->height / 2 * distanceRate) {
		return true;
	}
	return false;
}

float RectBody::getWidth() {
    return this->width * this->getScale();
}

float RectBody::getHeight() {
    return this->height * this->getScale();
}

void RectBody::setWidth(float width) {
    this->width = width / this->getScale();
}

void RectBody::setHeight(float height) {
    this->height = height / this->getScale();
}