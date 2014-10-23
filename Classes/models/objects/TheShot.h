#ifndef THE_SHOT_H
#define THE_SHOT_H

#include "AnimatedObject.h"

USING_NS_CC;

class TheShot : public AnimatedObject {
    UG_PROPERTY_FULL(float, strength, Strength);
    UG_PROPERTY_GETTER(int, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
    UG_PROPERTY_GETTER(RectBody*, rangeAttack, RangeAttack);
private:
    void initBody();
public:
    TheShot(int type, Point pos);
    virtual ~TheShot();
    void initFaces(std::string key, __Array* faceStates);
};

#endif
