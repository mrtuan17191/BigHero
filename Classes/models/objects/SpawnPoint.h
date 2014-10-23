#ifndef SPAWN_POINT_H
#define SPAWN_POINT_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class SpawnPoint : public Ref {
    UG_PROPERTY_GETTER(int, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
protected:
public:
    SpawnPoint(int type, Point pos);
    virtual ~SpawnPoint();
};

#endif