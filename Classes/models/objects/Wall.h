#ifndef WALL_H
#define WALL_H

#include "AnimatedObject.h"

USING_NS_CC;

class Wall : public AnimatedObject {
    UG_PROPERTY_GETTER(int, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
protected:
public:
    Wall(int type, Point pos);
    virtual ~Wall();
};

#endif