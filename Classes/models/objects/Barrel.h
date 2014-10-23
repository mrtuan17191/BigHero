#ifndef BARREL_H
#define BARREL_H

#include "AnimatedObject.h"

USING_NS_CC;

class Barrel : public AnimatedObject {
    UG_PROPERTY_GETTER(int, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
protected:
public:
    Barrel(int type, Point pos);
    virtual ~Barrel();
    void initBody();
};

#endif