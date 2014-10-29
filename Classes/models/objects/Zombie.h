#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "AnimatedObject.h"
#include "Situation.h"
#include "ThePoint.h"

USING_NS_CC;

class Zombie : public AnimatedObject {
    UG_PROPERTY_FULL(float, currentDelayAiming, CurrentDelayAiming);
    UG_PROPERTY_FULL(float, currentDelayDefending, CurrentDelayDefending);
    UG_PROPERTY_FULL(float, currentDelayPunching, CurrentDelayPunching);
    UG_PROPERTY_FULL(float, currentDelayAfterPunch, CurrentDelayAfterPunch);
    UG_PROPERTY_FULL(float, currentDelayHurting, CurrentDelayHurting);
    UG_PROPERTY_FULL(float, currentDelayGetUp, CurrentDelayGetUp);
    UG_PROPERTY_FULL(float, currentVectorY, CurrentVectorY);
    UG_PROPERTY_FULL(float, currentDelayToChangeDirection, CurrentDelayToChangeDirection);
    UG_PROPERTY_FULL(float, hp, HP);
    UG_PROPERTY_FULL(float, strength, Strength);
    UG_PROPERTY_FULL(float, vitality, Vitality);
    UG_PROPERTY_FULL(float, runVelocity, RunVelocity);
    UG_PROPERTY_FULL(float, attackableX, AttackableX);
    UG_PROPERTY_FULL(float, attackableY, AttackableY);
    UG_PROPERTY_FULL(float, halfPunchingTime, HalfPunchingTime);
    UG_PROPERTY_FULL(int, checkPoint, CheckPoint);
    UG_PROPERTY_GETTER(std::string, type, Type);
    UG_PROPERTY_GETTER(Point, pos, Pos);
    UG_PROPERTY_GETTER(RectBody*, rangeAttack, RangeAttack);
    UG_PROPERTY_FULL(Vector<ThePoint*>*, fallPositions, FallPositions);
    UG_PROPERTY_FULL_BOOL(hurtUp, HurtUp);
    UG_PROPERTY_FULL_BOOL(knockOut, KnockOut);
    UG_PROPERTY_FULL_BOOL(called, Called);
    UG_PROPERTY_FULL_BOOL(aiming, Aiming);
private:
    void initBody();
public:
    Zombie(std::string type, Point pos);
    Zombie(std::string type);
    virtual ~Zombie();
    void initFaces(std::string key, __Array* faceStates);
    void punch();
    void hurt();
    void ko();
    bool isDead();
    bool isFireType();
    bool isDriveType();
    bool isBlockType();
    bool isBossType();
    bool aim(float dt);
    void updateShadow();
};

#endif
