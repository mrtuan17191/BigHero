#ifndef OBJ_H
#define OBJ_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"
#include <cmath>

USING_NS_CC;

class Obj : public Ref {
protected:
    UG_PROPERTY_GETTER(int, objId, ObjId);
    UG_PROPERTY_FULL(Body*, body, Body);
    UG_PROPERTY_GETTER(__Array*, ranges, Ranges);
    UG_PROPERTY_GETTER(__Array*, faces, Faces);
public:
    Obj();
    virtual ~Obj();
    void update(float dt);
    void setPosition(cocos2d::Point position);
    cocos2d::Point getPosition();
    Face* getFace(char* faceName);
    void removeFace(char* faceName);
    void setState(char* stateName, char* faceName);
};

#endif
