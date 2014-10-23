#ifndef FACE_STATE_H
#define FACE_STATE_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class FaceState : public Ref {
    UG_PROPERTY_FULL(string, stateName, StateName);
    UG_PROPERTY_FULL_BOOL(loop, Loop);
protected:
public:
    FaceState(std::string stateName, bool loop);
    virtual ~FaceState();
};

#endif