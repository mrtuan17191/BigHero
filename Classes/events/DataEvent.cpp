#include "DataEvent.h"

DataEvent::DataEvent() {
    this->argumentInt = -1;
    this->argumentFloat = -1;
    this->argumentFloat2 = -1;
    this->argumentPoint = Point::ZERO;
    this->argumentReference = NULL;
    this->argumentObject = NULL;
}

DataEvent::~DataEvent() {
    CC_SAFE_RELEASE(this->argumentObject);
}