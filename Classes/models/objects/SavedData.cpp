#include "SavedData.h"

SavedData::SavedData() {
    this->autorelease();
    this->musicOn = true;
    this->sfxOn = true;
    this->sharedSocial = false;
    this->level = -1;
}

SavedData::~SavedData() {
}