#include "TheShot.h"

TheShot::TheShot(int type, Point pos) {
    this->type = type;
    this->pos = pos;
    if (this->type == SHOT_TYPE_LASER) {
        this->strength = 15;
    } else {
        this->strength = 0;
    }
    this->rangeAttack = NULL;
    this->initBody();
}

TheShot::~TheShot() {
}

void TheShot::initBody() {
    Face* shadowFace = new Face();
    shadowFace->setName((char*) FACE_SHADOW);
    shadowFace->initWithSpriteFrameName(ANI_SHADOW_1);
    shadowFace->setOpacity(150);
    shadowFace->getTexture()->setAliasTexParameters();
    this->faces->addObject(shadowFace);
    
    RectBody* body = new RectBody(shadowFace->getContentSize().width * 0.25f, shadowFace->getContentSize().height * 0.25f);
    CC_SAFE_RETAIN(body);
    this->body = body;
    this->body->setScale(CONF_SCALE_GAME);
    
    this->rangeAttack = new RectBody(body->getWidth(), 40);
    CC_SAFE_RETAIN(this->rangeAttack);
}

void TheShot::initFaces(std::string key, __Array* faceStates) {
    AnimatedObject::initFaces(key, faceStates);
    Face* charFace = this->getFace((char*) FACE_CHAR);
    charFace->setOOVector(Point(0, 20));
    charFace->setState(charFace->getState(STATE_IDLE));
}