#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "AnimatedObject.h"

USING_NS_CC;

class Elevator : public AnimatedObject {
    UG_PROPERTY_GETTER(int, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
protected:
public:
    Elevator(int type, Point pos);
    virtual ~Elevator();
};

#endif