#ifndef SITUATION_H
#define SITUATION_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;
using namespace std;

class Situation : public Ref {
    UG_PROPERTY_GETTER(Situation*, subSituation, SubSituation);
    UG_PROPERTY_GETTER(__Dictionary*, data, Data);
    UG_PROPERTY_FULL(void*, target, Target);
    UG_PROPERTY_FULL(int, situationType, SituationType);
private:
    virtual ~Situation();
public:
    Situation();
    void raiseSituation(Situation* situation);
    void endSubSituation();
};

#endif
