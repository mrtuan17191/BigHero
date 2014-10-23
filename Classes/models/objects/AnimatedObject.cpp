#include "AnimatedObject.h"

AnimatedObject::AnimatedObject() {
    this->listStates = new Vector<__String*>();
}

AnimatedObject::~AnimatedObject() {
    CC_SAFE_DELETE(this->listStates);
}

void AnimatedObject::initFaces(std::string key, __Array* faceStates) {
    this->key = key;
    Face* charFace = new Face();
    charFace->setName((char*) FACE_CHAR);
    charFace->initFaceForKey(this->key, faceStates);
    this->faces->addObject(charFace);
    
    __Array* allKeys = charFace->getStates()->allKeys();
    if (allKeys != NULL) {
        for (int i = 0; i < allKeys->count(); i++) {
            __String* keyString = (__String*) allKeys->getObjectAtIndex(i);
            this->listStates->pushBack(__String::create(keyString->getCString()));
        }
    }
}