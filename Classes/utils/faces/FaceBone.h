#ifndef FACE_BONE_H
#define FACE_BONE_H

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "spine/spine-cocos2dx.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "spine/spine-cocos2dx.h"
#endif
#include "../UGCustomMacros.h"
#include "Face.h"

USING_NS_CC;
using namespace spine;

class FaceBone: public SkeletonAnimation {
public:
    FaceBone(const char* jsonFile, const char* atlasFile, float scale = 1.0f);
    virtual void setAnimation (const char* name, bool loop, int stateIndex = 0);
    std::string getAnimationRunning() {
        return this->animationRunning;
    }
private:
    std::string animationRunning;
    
    virtual void onEnter();
    virtual void onExit();
};
#endif
