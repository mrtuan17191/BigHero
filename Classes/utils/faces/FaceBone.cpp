#include "FaceBone.h"

FaceBone::FaceBone(const char* jsonFile, const char* atlasFile, float scale) : SkeletonAnimation(jsonFile, atlasFile, scale) {
    this->animationRunning = "";
    this->autorelease();
}

void FaceBone::setAnimation(const char* name, bool loop, int stateIndex) {
    if (strcmp(this->animationRunning.data(), name) != 0) {
        this->animationRunning = name;
        SkeletonAnimation::setAnimation(0,name, loop);
    }
}

void FaceBone::onEnter() {
    SkeletonAnimation::onEnter();
}

void FaceBone::onExit() {
    SkeletonAnimation::onExit();
    this->removeAllChildrenWithCleanup(true);
}