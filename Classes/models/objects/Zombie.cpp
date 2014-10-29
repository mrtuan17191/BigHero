#include "Zombie.h"

Zombie::Zombie(string type) {
    this->currentDelayAiming = 0;
    this->currentDelayDefending = 0;
    this->currentDelayPunching = 0;
    this->currentDelayAfterPunch = 0;
    this->currentDelayHurting = 0;
    this->currentDelayGetUp = 0;
    this->currentVectorY = 0;
    this->currentDelayToChangeDirection = 0;
    this->hp = 0;
    this->strength = 0;
    this->vitality = 0;
    this->runVelocity = 0;
    this->attackableX = 0;
    this->attackableY = 0;
    this->halfPunchingTime = 0;
    this->checkPoint = -1;
    this->type = type;
    this->pos = Point::ZERO;
    this->rangeAttack = NULL;
    this->fallPositions = new Vector<ThePoint*>();
    this->hurtUp = true;
    this->knockOut = false;
    this->called = false;
    this->aiming = false;

    this->initBody();
}

Zombie::Zombie(string type, Point pos)  {
    this->type = type;
    this->pos = pos;
}

Zombie::~Zombie() {
    CC_SAFE_DELETE(this->fallPositions);
    Utils::safeRelease(this->rangeAttack);
}

void Zombie::initBody() {
    string frameName = ANI_SHADOW_2;
    if (this->isDriveType() || this->isBossType()) {
        frameName = ANI_SHADOW_3;
    }
    Face* shadowFace = new Face();
    shadowFace->setName((char*) FACE_SHADOW);
    shadowFace->initWithSpriteFrameName(frameName);
    shadowFace->setOpacity(150);
    shadowFace->getTexture()->setAliasTexParameters();
    this->faces->addObject(shadowFace);
    
    RectBody* body = new RectBody(shadowFace->getContentSize().width * 0.25f, shadowFace->getContentSize().height * 0.25f);
    CC_SAFE_RETAIN(body);
    this->body = body;
    this->body->setScale(CONF_SCALE_GAME);
    
    if (this->isFireType()) {
        this->rangeAttack = new RectBody(body->getWidth() * 4, 0);
        CC_SAFE_RETAIN(this->rangeAttack);
        
        this->attackableX = body->getWidth() * 4;
        this->attackableY = 5;
        this->hp = 100;
        this->strength = 15;
        this->vitality = 1;
        this->runVelocity = 60;
    } else if (this->isDriveType()) {
        this->rangeAttack = new RectBody(body->getWidth() / 2, 1);
        CC_SAFE_RETAIN(this->rangeAttack);
        
        this->strength = 15;
        this->runVelocity = 100;
    } else {
        this->rangeAttack = new RectBody(body->getWidth() / 2, 40);
        CC_SAFE_RETAIN(this->rangeAttack);
        
        this->attackableX = 5;
        this->attackableY = 5;
        this->hp = 100;
        this->strength = 15;
        this->vitality = 1;
        this->runVelocity = 60;
    }
    
    //BOSS
    if (this->getType() == KEY_BOSS1) {
        this->attackableX = 30;
        this->attackableY = 5;
        
        this->runVelocity = 50;
    }
}

void Zombie::initFaces(std::string key, __Array* faceStates) {
    AnimatedObject::initFaces(key, faceStates);
    Face* charFace = this->getFace((char*) FACE_CHAR);
    charFace->setState(charFace->getState(STATE_IDLE));
    
    float minVector = CONF_MAN_MIN_VECTOR_Y;
    if (this->isDriveType()) {
        minVector = 35;
    }
    if (this->isBossType()) {
        minVector = 28;
    }
    charFace->setOOVector(Point(0, minVector));
    this->currentVectorY = minVector;
}

void Zombie::punch() {
    if (this->currentDelayPunching <= 0) {
        Face* charFace = this->getFace((char*) FACE_CHAR);
        float duration = ((ActionInterval*) charFace->getState(STATE_ATTACK))->getDuration();
        this->currentDelayPunching = duration;
        this->halfPunchingTime = this->currentDelayPunching / 2;
        
        if (this->isFireType()) {
            this->currentDelayPunching += 0.5f;
        }
    }
}

bool Zombie::aim(float dt) {
    if (this->currentDelayAiming > 0) {
        this->currentDelayAiming -= dt;
    } else {
        this->currentDelayAiming = CONF_DELAY_ZOMBIE_AIMING;
        return true;
    }
    return false;
}

void Zombie::hurt() {
    this->hurtUp = !this->hurtUp;
    this->currentDelayHurting = CONF_DELAY_HURTING;
    this->currentDelayPunching = 0;
    this->currentDelayAfterPunch = 0;
}

void Zombie::ko() {
    if (this->body->isFlipX()) {
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x + 20, CONF_MAN_MIN_VECTOR_Y + 20));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x + 30, CONF_MAN_MIN_VECTOR_Y));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x + 35, CONF_MAN_MIN_VECTOR_Y + 5));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x + 37, CONF_MAN_MIN_VECTOR_Y));
    } else {
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x - 20, CONF_MAN_MIN_VECTOR_Y + 20));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x - 30, CONF_MAN_MIN_VECTOR_Y));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x - 35, CONF_MAN_MIN_VECTOR_Y + 5));
        this->fallPositions->pushBack(new ThePoint(this->getPosition().x - 37, CONF_MAN_MIN_VECTOR_Y));
    }
    this->knockOut = true;
    this->currentDelayHurting = CONF_DELAY_KOING;
    this->currentDelayPunching = 0;
    this->currentDelayAfterPunch = 0;
    
    Face* charFace = this->getFace((char*) FACE_CHAR);
    float duration = ((ActionInterval*) charFace->getState(STATE_GET_UP))->getDuration();
    this->currentDelayGetUp = this->currentDelayHurting - duration;
}

bool Zombie::isDead() {
    return this->knockOut && this->hp <= 0;
}

void Zombie::updateShadow() {
    float distancePercent = 1 - ((float) (this->getCurrentVectorY() -  CONF_MAN_MIN_VECTOR_Y) / (CONF_MAN_MAX_VECTOR_Y - CONF_MAN_MIN_VECTOR_Y));
    distancePercent = (distancePercent * (CONF_SHADOW_MAX - CONF_SHADOW_MIN)) + CONF_SHADOW_MIN;
    Face* shadowFace = this->getFace((char*) FACE_SHADOW);
    shadowFace->setScale(this->getBody()->getScale() * 0.25f * distancePercent);
}

bool Zombie::isFireType() {
    if (this->type == KEY_LASER_NERD) {
        return true;
    }
    return false;
}

bool Zombie::isDriveType() {
    if (this->type == KEY_FORKLIFT) {
        return true;
    }
    return false;
}

bool Zombie::isBlockType() {
    if(this->type == KEY_RIOT_COP) {
        return true;
    }
    return false;
}

bool Zombie::isBossType() {
    if (this->type == KEY_BOSS1 || this->type == KEY_BOSS2 || this->type == KEY_BOSS3 || this->type == KEY_BOSS4 || this->type == KEY_BOSS5) {
        return true;
    }
    return false;
}
