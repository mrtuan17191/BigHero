#include "Man.h"

Man::Man() {
    this->currentPunch = 0;
    this->currentVectorY = 0;
    this->currentDelayPunching = 0;
    this->currentDelayToNextPunch = 0;
    this->currentDelayHurting = 0;
    this->targetVectorY = 0;
    this->halfPunchingTime = 0;
    this->hp = 0;
    this->strength = 0;
    this->vitality = 0;
    this->ap = 0;
    this->wp = 0;
    this->rangeAttack = NULL;
    this->punchState = "";
    this->fallPositions = new Vector<ThePoint*>();
    this->appear = true;
    this->disappear = false;
    this->knockOut = 0;
    this->transformer = false;

    this->initBody();
}

Man::~Man() {
    Utils::safeRelease(this->rangeAttack);
    CC_SAFE_DELETE(this->fallPositions);
}

void Man::initBody() {
    Face* shadowFace = new Face();
    shadowFace->setName((char*) FACE_SHADOW);
    shadowFace->initWithSpriteFrameName(ANI_SHADOW_2);
    shadowFace->setOpacity(150);
    shadowFace->getTexture()->setAliasTexParameters();
    this->faces->addObject(shadowFace);
    
    RectBody* body = new RectBody(shadowFace->getContentSize().width * 0.25f, shadowFace->getContentSize().height * 0.25f);
    CC_SAFE_RETAIN(body);
    this->body = body;
    this->body->setScale(CONF_SCALE_GAME);

    this->rangeAttack = new RectBody(body->getWidth() / 2, 40);
    CC_SAFE_RETAIN(this->rangeAttack);
    
    this->hp = CONF_MAN_HP;
    this->strength = 15;
    this->vitality = 5;
}

void Man::initFaces(std::string key, __Array* faceStates) {
    AnimatedObject::initFaces(key, faceStates);
    Face* charFace = this->getFace((char*) FACE_CHAR);
    charFace->setOOVector(Point(0, CONF_MAN_MIN_VECTOR_Y));
    this->currentVectorY = CONF_MAN_MIN_VECTOR_Y;
    charFace->setState(charFace->getState(STATE_IDLE));
    
    FiniteTimeAction* tintActionRed = TintTo::create(0.5f, 255, 0, 0);
    FiniteTimeAction* tintActionPink = TintTo::create(0.5f, 212, 112, 95);
    Sequence* sequenceTint = Sequence::create(tintActionRed, tintActionPink, NULL);
    FiniteTimeAction* repeatSequence = RepeatForever::create(sequenceTint);
    charFace->registerState(STATE_FRENZY_ON, repeatSequence);
    
    FiniteTimeAction* tintActionWhite = TintTo::create(0.0f, 255, 255, 255);
    charFace->registerState(STATE_FRENZY_OFF, tintActionWhite);
}

void Man::run(float velocity, float angle) {
    if (this->appear || this->disappear) {
        return;
    }
    
    if (this->currentDelayPunching == -1) {
        return;
    }
    if (this->isDead()) {
        this->getBody()->setVelocity(Point::ZERO);
        return;
    }
    Point manVelocity = Point(velocity * cos(angle), velocity * sin(angle));
    this->getBody()->setVelocity(manVelocity);
}

void Man::punch() {
    if (this->appear || this->disappear) {
        return;
    }
    
    if (this->currentDelayPunching == -1) { // aerial attacking
        return;
    }
    
    if (this->currentDelayHurting > 0) { // hurting
        return;
    }
    
    if (this->isDead()) { // dead
        return;
    }
    
    float centerOfJumpDistance = (CONF_MAN_MAX_VECTOR_Y - CONF_MAN_MIN_VECTOR_Y) / 2;
    if (this->currentVectorY > centerOfJumpDistance) {
        this->punchState = (char*) STATE_AERIAL_ATTACK;
        this->currentDelayPunching = -1;
        this->currentPunch = 0;
        return;
    }
    
    if (this->currentVectorY != CONF_MAN_MIN_VECTOR_Y) {
        return;
    }
    
    if (this->currentDelayPunching <= 0) {
        if (this->currentDelayToNextPunch > 0) {
            this->currentPunch++;
            if (this->currentPunch > 5) {
                this->currentPunch = 1;
            }
        } else {
            this->currentPunch = 1;
        }

        char* stateAttack = (char*) "";
        if (this->isTransformer()) {
            if (this->currentPunch == 1) {
                stateAttack = (char*) STATE_FRENZY_ATTACK_1;
            } else if (this->currentPunch == 2) {
                stateAttack = (char*) STATE_FRENZY_ATTACK_2;
            } else if (this->currentPunch == 3) {
                stateAttack = (char*) STATE_FRENZY_ATTACK_3;
            } else if (this->currentPunch == 4) {
                stateAttack = (char*) STATE_FRENZY_ATTACK_4;
            } else if (this->currentPunch == 5) {
                stateAttack = (char*) STATE_FRENZY_ATTACK_5;
            }
        } else {
            if (this->currentPunch == 1) {
                stateAttack = (char*) STATE_ATTACK_1;
            } else if (this->currentPunch == 2) {
                stateAttack = (char*) STATE_ATTACK_2;
            } else if (this->currentPunch == 3) {
                stateAttack = (char*) STATE_ATTACK_3;
            } else if (this->currentPunch == 4) {
                stateAttack = (char*) STATE_ATTACK_4;
            } else if (this->currentPunch == 5) {
                stateAttack = (char*) STATE_ATTACK_5;
            }
        }
        
        
        Face* charFace = this->getFace((char*) FACE_CHAR);
        float duration = ((ActionInterval*) charFace->getState(stateAttack))->getDuration();
        this->punchState = stateAttack;
        this->currentDelayPunching = duration;
        this->halfPunchingTime = this->currentDelayPunching / 2;
    }
}

void Man::jump() {
    if (this->appear || this->disappear) {
        return;
    }
    
    if (this->isDead()) { // dead
        return;
    }
    
    if (this->targetVectorY == 0) {
        this->targetVectorY = CONF_MAN_MAX_VECTOR_Y;
    }
}

void Man::hurt() {
    this->currentDelayHurting = CONF_DELAY_HURTING;
    this->currentDelayPunching = 0;
    this->punchState = "";
}

void Man::ko() {
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
    this->punchState = "";
}

bool Man::isDead() {
    return this->knockOut && this->hp <= 0;
}

void Man::updateShadow() {
    float distancePercent = 1 - ((float) (this->getCurrentVectorY() -  CONF_MAN_MIN_VECTOR_Y) / (CONF_MAN_MAX_VECTOR_Y - CONF_MAN_MIN_VECTOR_Y));
    distancePercent = (distancePercent * (CONF_SHADOW_MAX - CONF_SHADOW_MIN)) + CONF_SHADOW_MIN;
    Face* shadowFace = this->getFace((char*) FACE_SHADOW);
    shadowFace->setScale(this->getBody()->getScale() * 0.25f * distancePercent);
}