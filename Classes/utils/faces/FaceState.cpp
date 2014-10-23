#include "FaceState.h"

FaceState::FaceState(string stateName, bool loop) {
    this->autorelease();
    this->stateName = stateName;
    this->loop = loop;
}

FaceState::~FaceState() {
}