#include "Barrel.h"

Barrel::Barrel(int type, Point pos) {
    this->type = type;
    this->pos = pos;
    this->initBody();
}

Barrel::~Barrel() {
}

void Barrel::initBody() {
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
    this->body->setPosition(this->pos);
    
    Face* faceMain = new Face();
    faceMain->setName((char*) FACE_MAIN);
    faceMain->initWithSpriteFrameName(ANI_BARREL);
    faceMain->setOOVector(Point(0, 15));
    faceMain->getTexture()->setAliasTexParameters();
    this->faces->addObject(faceMain);
}