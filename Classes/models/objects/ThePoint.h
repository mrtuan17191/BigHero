#ifndef THE_POINT_H
#define THE_POINT_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class ThePoint : public Ref {
    UG_PROPERTY_FULL(float, x, X);
    UG_PROPERTY_FULL(float, y, Y);
protected:
public:
    ThePoint(float x, float y);
    virtual ~ThePoint();
};

#endif