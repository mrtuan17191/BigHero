#ifndef ANIMATED_OBJECT_H
#define ANIMATED_OBJECT_H

#include "Obj.h"

USING_NS_CC;

class AnimatedObject : public Obj {
    UG_PROPERTY_FULL(Vector<__String*>*, listStates, ListStates);
protected:
    std::string key;
public:
    AnimatedObject();
    virtual ~AnimatedObject();
    void initFaces(std::string key, __Array* faceStates);
};

#endif
