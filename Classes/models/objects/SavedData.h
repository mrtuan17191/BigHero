#ifndef SAVED_DATA_H
#define SAVED_DATA_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class SavedData : public Ref {
    UG_PROPERTY_FULL_BOOL(sfxOn, SfxOn);
    UG_PROPERTY_FULL_BOOL(musicOn, MusicOn);
    UG_PROPERTY_FULL_BOOL(sharedSocial, SharedSocial);
    UG_PROPERTY_FULL(int, level, Level);
protected:
public:
    SavedData();
    virtual ~SavedData();
};

#endif