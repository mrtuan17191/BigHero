#ifndef MAN_H
#define MAN_H

#include "AnimatedObject.h"
#include "ThePoint.h"

USING_NS_CC;

class Man : public AnimatedObject {
    UG_PROPERTY_FULL(int, currentPunch, CurrentPunch);
    UG_PROPERTY_FULL(float, currentVectorY, CurrentVectorY);
    UG_PROPERTY_FULL(float, currentDelayPunching, CurrentDelayPunching);
    UG_PROPERTY_FULL(float, currentDelayToNextPunch, CurrentDelayToNextPunch);
    UG_PROPERTY_FULL(float, currentDelayHurting, CurrentDelayHurting);
    UG_PROPERTY_FULL(float, targetVectorY, TargetVectorY);
    UG_PROPERTY_FULL(float, halfPunchingTime, HalfPunchingTime);
    UG_PROPERTY_FULL(float, hp, HP);
    UG_PROPERTY_FULL(float, strength, Strength);
    UG_PROPERTY_FULL(float, vitality, Vitality);
    UG_PROPERTY_FULL(float, ap, AP);
    UG_PROPERTY_FULL(float, wp, WP);
    UG_PROPERTY_GETTER(RectBody*, rangeAttack, RangeAttack);
    UG_PROPERTY_FULL(std::string, punchState, PunchState);
    UG_PROPERTY_FULL(Vector<ThePoint*>*, fallPositions, FallPositions);
    UG_PROPERTY_FULL_BOOL(appear, Appear);
    UG_PROPERTY_FULL_BOOL(disappear, Disappear);
    UG_PROPERTY_FULL_BOOL(knockOut, KnockOut);
    UG_PROPERTY_FULL_BOOL(transformer, Transformer);
private:
    void initBody();
public:
    Man();
    virtual ~Man();
    void initFaces(std::string key, __Array* faceStates);
    void punch();
    void jump();
    void run(float velocity, float angle);
    void hurt();
    void ko();
    bool isDead();
    void updateShadow();
};

#endif
