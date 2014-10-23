#include "Level.h"

Level::Level(int level) {
    this->autorelease();
    this->level = level;
    this->man = NULL;
    
    this->stops = new Vector<__Float*>();
    this->spawnPoints = new Vector<SpawnPoint*>();
    this->walls = new Vector<Wall*>();
    this->elevators = new Vector<Elevator*>();
    this->barrels = new Vector<Barrel*>();
    this->zombies = new Vector<Zombie*>();
    this->shots = new Vector<TheShot*>();
    this->endZoneBody = NULL;
    this->airX = 0;
    this->currentCheckPoint = 0;
    this->cameraTarget = 0;
    this->reachCheckPoint = false;
    this->gameTime = 0;
    this->score = 0;
}

Level::~Level() {
    Utils::safeRelease(this->man);
    Utils::safeRelease(this->endZoneBody);
    CC_SAFE_DELETE(this->stops);
    CC_SAFE_DELETE(this->spawnPoints);
    CC_SAFE_DELETE(this->walls);
    CC_SAFE_DELETE(this->elevators);
    CC_SAFE_DELETE(this->barrels);
    CC_SAFE_DELETE(this->zombies);
    CC_SAFE_DELETE(this->shots);
}

void Level::update(float dt) {
    GameModel* gameModel = GameModel::getInstance();

    // update game time
    this->gameTime += dt;
    
    if (this->man->isTransformer()) {
        float newAngry = this->man->getAP() - (CONF_DOWN_AP_SPEED * dt);
        if (newAngry <= 0) {
            newAngry = 0;
            this->man->setTransformer(false);
            Face* charFace = this->man->getFace((char*) FACE_CHAR);
            charFace->getActionManager()->removeAction(charFace->getState(STATE_FRENZY_ON));
            charFace->runAction(charFace->getState(STATE_FRENZY_OFF));
        }
        this->man->setAP(newAngry);
    }
    
    // update zombies
    for (int i = 0; i < this->zombies->size(); i++) {
        Zombie* zombie = this->zombies->at(i);
        this->updateZombie(zombie, dt);
    }
    
    // update barrels
    for (int i = 0; i < this->barrels->size(); i++) {
        Barrel* barrel = this->barrels->at(i);
        barrel->update(dt);
    }
    
    // update shots
    for (int i = 0; i < this->shots->size(); i++) {
        TheShot* shot = this->shots->at(i);
        shot->update(dt);
        
        RectBody* attackRange = shot->getRangeAttack();
        attackRange->setPosition(shot->getPosition());
        if (Utils::b2Interact(attackRange, this->man->getBody())) {
            this->registerDamage(attackRange, shot, DAMAGE_SHOT);
        }
    }
    
    // get old position X for update camera
    float oldPosX = this->man->getPosition().x;
    
    // update man
    this->updateMan(dt);
    
    // update camera in 2 situations:
    // when camara target equals 0, update camera follow man, man can't go back
    // when camara target > 0, update camera to correct target
    RectBody* manBody = (RectBody*) this->man->getBody();
    float moreDistanceX = this->man->getPosition().x - oldPosX;
    float realWidth = this->blockRight - this->blockLeft;
    float darkX = realWidth - gameModel->getDisplayResolutionSize().width;
    
    float distanceWidth = this->blockRight - this->blockLeft;
    float distanceToFollowX = (distanceWidth - darkX) / 2;
    
    if (this->cameraTarget == 0) {
        if (distanceToFollowX > 0) {
            float distanceMinX = manBody->getPosition().x - this->blockLeft - distanceToFollowX;
            if (distanceMinX > 0) {
                float distanceMaxX = this->blockRight - manBody->getPosition().x;
                if (distanceMaxX > distanceToFollowX) {
                    if (distanceMinX > this->airX) {
                        this->airX = distanceMinX;
                    }
                }
            }
        }
    } else {
        if (this->currentCheckPoint < this->stops->size() - 1) {
            this->cameraTarget += moreDistanceX;
        } else {
            this->cameraTarget = this->blockRight - this->blockLeft - distanceToFollowX * 2;
        }
        this->airX = this->airX + CONF_AIR_TO_TARGET_VELOCITY * dt;
        if (this->airX > this->cameraTarget) {
            this->airX = this->cameraTarget;
            this->cameraTarget = 0;
        }
    }
    
    // call enemy
    if (!this->reachCheckPoint) {
        float maxDarkX = this->blockRight - distanceToFollowX;
        if (manBody->getPosition().x >= maxDarkX) {
            this->reachCheckPoint = true;
            this->callEnemiesAtCheckPoint();
        }
    }
    
    // finish
    if (this->endZoneBody != NULL) {
        if (Utils::b2Interact(this->man->getBody(), this->endZoneBody)) {
            this->man->setDisappear(true);
            this->man->getBody()->setVelocity(Point(CONF_MAN_DISAPPEAR_VELOCITY, 0));
        }
    }
    
    if (this->reachCheckPoint) {
        bool noMoreZombie = true;
        for (int i = 0; i < this->zombies->size(); i++) {
            Zombie* zombie = this->zombies->at(i);
            if (zombie->isCalled()) {
                noMoreZombie = false;
                break;
            }
        }
        
        if (noMoreZombie) {
            GameEvent* gameEvent = new GameEvent();
            gameEvent->setEventCode(EVT_NEXT_CHECK_POINT);
            gameModel->fireEvent(gameEvent);
            this->reachCheckPoint = false;
        }
    }
    
    // update appearing
    if (this->man->isAppear()) {
        if (this->man->getPosition().x > CONF_DISTANCE_TO_APPEAR) {
            this->man->setAppear(false);
            manBody->setVelocity(Point::ZERO);
        }
    }
}

void Level::updateMan(float dt) {
    this->man->update(dt);
    RectBody* manBody = (RectBody*) this->man->getBody();
    char* manState = (char*) STATE_IDLE;
    
    // update man flip and state
    if (manBody->getVelocity().x > 0) {
        manBody->setFlipX(false);
        manState = (char*) STATE_RUN;
    } else if (manBody->getVelocity().x < 0) {
        manBody->setFlipX(true);
        manState = (char*) STATE_RUN;
    }
    
    // update limit, limit left always change when camera moving
    if (!this->man->isAppear()) {
        float leftManPos = this->man->getPosition().x - manBody->getWidth() / 2;
        float rightManPos = this->man->getPosition().x + manBody->getWidth() / 2;
        float topManPos = this->man->getPosition().y + manBody->getHeight() / 2;
        float bottomManPos = this->man->getPosition().y - manBody->getHeight() / 2;
        
        if (leftManPos < this->blockLeft + this->airX) {
            this->man->setPosition(Point(this->blockLeft + this->airX + manBody->getWidth() / 2, this->man->getPosition().y));
        }
        if (rightManPos > this->blockRight) {
            this->man->setPosition(Point(this->blockRight - manBody->getWidth() / 2, this->man->getPosition().y));
        }
        if (topManPos > this->blockUp) {
            this->man->setPosition(Point(this->man->getPosition().x, this->blockUp - manBody->getHeight() / 2));
        }
        if (bottomManPos < this->blockDown) {
            this->man->setPosition(Point(this->man->getPosition().x, this->blockDown + manBody->getHeight() / 2));
        }
    }
    
    // update time to next skill punch of man
    if (this->man->getCurrentDelayToNextPunch() > 0) {
        float delayToNextPunch = this->man->getCurrentDelayToNextPunch();
        this->man->setCurrentDelayToNextPunch(delayToNextPunch - dt);
        if (this->man->getCurrentDelayToNextPunch() <= 0) {
            manBody->setFreeze(false);
        }
    }
    
    // update time man being knock out
    // man will fall and get up if he still alive
    bool isHurt = false;
    if (this->man->getCurrentDelayHurting() > 0) {
        isHurt = true;
        manBody->setFreeze(true);
        if (this->man->isKnockOut()) {
            manState = (char*) STATE_KO;
            
            if (this->man->getFallPositions()->size() > 0) {
                ThePoint* point = this->man->getFallPositions()->at(0);
                Face* faceChar = this->man->getFace((char*) FACE_CHAR);
                
                if (this->man->getCurrentVectorY() < point->getY()) {
                    float newVectorY = this->man->getCurrentVectorY() + CONF_FALL_VELOCITY_Y * dt;
                    if (newVectorY > point->getY()) {
                        newVectorY = point->getY();
                    }
                    this->man->setCurrentVectorY(newVectorY);
                    faceChar->setOOVector(Point(0, newVectorY));
                }
                if (this->man->getCurrentVectorY() > point->getY()) {
                    float newVectorY = this->man->getCurrentVectorY() - CONF_FALL_VELOCITY_Y * dt;
                    if (newVectorY < point->getY()) {
                        newVectorY = point->getY();
                    }
                    this->man->setCurrentVectorY(newVectorY);
                    faceChar->setOOVector(Point(0, newVectorY));
                }
                if (this->man->getPosition().x < point->getX()) {
                    float newX = this->man->getPosition().x + CONF_FALL_VELOCITY_X * dt;
                    if (newX > point->getX()) {
                        newX = point->getX();
                    }
                    this->man->setPosition(Point(newX, this->man->getPosition().y));
                }
                if (this->man->getPosition().x > point->getX()) {
                    float newX = this->man->getPosition().x - CONF_FALL_VELOCITY_X * dt;
                    if (newX < point->getX()) {
                        newX = point->getX();
                    }
                    this->man->setPosition(Point(newX, this->man->getPosition().y));
                }
                
                if (point->getX() == this->man->getPosition().x && point->getY() == this->man->getCurrentVectorY()) {
                    this->man->getFallPositions()->erase(0);
                }
            } else {
                if (!this->man->isDead()) {
                    float delayToHurting = this->man->getCurrentDelayHurting();
                    this->man->setCurrentDelayHurting(delayToHurting - dt);
                }
            }
        } else {
            float delayToHurting = this->man->getCurrentDelayHurting();
            this->man->setCurrentDelayHurting(delayToHurting - dt);
            manState = (char*) STATE_HIT;
        }
        
        if (this->man->getCurrentDelayHurting() < 0) {
            this->man->setKnockOut(false);
            manBody->setFreeze(false);
            isHurt = false;
        }
    }
    
    if (!isHurt) {
        // update time man punching
        // update man aerial attack
        // jumping
        if (this->man->getCurrentDelayPunching() > 0) {
            float delayToPunching = this->man->getCurrentDelayPunching();
            this->man->setCurrentDelayPunching(delayToPunching - dt);
            manBody->setFreeze(true);
            if (this->man->getCurrentDelayPunching() < this->man->getHalfPunchingTime()
                && this->man->getHalfPunchingTime() != 0) {
                RectBody* attackRange = this->man->getRangeAttack();
                if (this->man->getBody()->isFlipX()) {
                    attackRange->setPosition(this->man->getPosition() - Point(attackRange->getWidth() / 2, 0));
                } else {
                    attackRange->setPosition(this->man->getPosition() + Point(attackRange->getWidth() / 2, 0));
                }
                this->registerDamage(attackRange, this->man, DAMAGE_MAN);
                this->man->setHalfPunchingTime(0);
            }
            
            if (this->man->getCurrentDelayPunching() <= 0) {
                this->man->setCurrentDelayToNextPunch(CONF_DELAY_TO_NEXT_PUNCH);
                this->man->setPunchState("");
            }
        } else if (this->man->getCurrentDelayPunching() == -1) {
            float speedAerial = CONF_SPEED_JUMP * 1.5f;
            if (this->man->getBody()->isFlipX()) {
                speedAerial = -speedAerial;
            }
            this->man->getBody()->setVelocity(Point(speedAerial, 0));
            this->man->setCurrentVectorY(this->man->getCurrentVectorY() - abs(speedAerial) * dt);
            if (this->man->getCurrentVectorY() < CONF_MAN_MIN_VECTOR_Y) {
                this->man->setCurrentVectorY(CONF_MAN_MIN_VECTOR_Y);
                this->man->setTargetVectorY(0);
                this->man->setCurrentDelayPunching(0);
                this->man->setPunchState("");
                this->man->getBody()->setVelocity(Point::ZERO);
                
                RectBody* attackRange = this->man->getRangeAttack();
                RectBody* attackRangeLarger = new RectBody(attackRange->getWidth() * 2, attackRange->getHeight());
                attackRangeLarger->setPosition(this->man->getPosition());
                this->registerDamage(attackRangeLarger, this->man, DAMAGE_MAN);
            }
        } else {
            if (this->man->getTargetVectorY() == CONF_MAN_MAX_VECTOR_Y) {
                this->man->setCurrentVectorY(this->man->getCurrentVectorY() + CONF_SPEED_JUMP * dt);
                if (this->man->getCurrentVectorY() > this->man->getTargetVectorY()) {
                    this->man->setCurrentVectorY(this->man->getTargetVectorY());
                    this->man->setTargetVectorY(CONF_MAN_MIN_VECTOR_Y);
                }
                manState = (char*) STATE_JUMP;
            } else if (this->man->getTargetVectorY() == CONF_MAN_MIN_VECTOR_Y) {
                this->man->setCurrentVectorY(this->man->getCurrentVectorY() - CONF_SPEED_JUMP * dt);
                if (this->man->getCurrentVectorY() < this->man->getTargetVectorY()) {
                    this->man->setCurrentVectorY(this->man->getTargetVectorY());
                    this->man->setTargetVectorY(0);
                }
                manState = (char*) STATE_JUMP;
            }
        }
    }

    // update scale shadow
    this->man->updateShadow();
    
    // update jumping char faces, always
    Face* charFace = this->man->getFace((char*) FACE_CHAR);
    charFace->setOOVector(Point(charFace->getOOVector().x, this->man->getCurrentVectorY()));
    
    // update man's animation, always
    if (this->man->getPunchState() != "") {
        char punchState[200];
        sprintf(punchState, "%s", this->man->getPunchState().data());
        this->man->setState(punchState, (char*) FACE_CHAR);
    } else {
        this->man->setState(manState, (char*) FACE_CHAR);
    }
}

void Level::updateZombie(Zombie* zombie, float dt) {
    GameModel* gameModel = GameModel::getInstance();
    zombie->update(dt);
    if (!zombie->isCalled()) {
        zombie->getBody()->setVisible(false);
        return;
    }
    
    char* zombieState = (char*) STATE_IDLE;
    RectBody* manBody = (RectBody*) this->man->getBody();
    RectBody* zombieBody = (RectBody*) zombie->getBody();
    
    // check zombie position has touch limit above or limit below
    bool isTouchLimit = false;
    float topZombiePos = zombie->getPosition().y + zombieBody->getHeight() / 2;
    float bottomZombiePos = zombie->getPosition().y - zombieBody->getHeight() / 2;
    
    if (topZombiePos > this->blockUp) {
        zombie->setPosition(Point(zombie->getPosition().x, this->blockUp - zombieBody->getHeight() / 2));
        isTouchLimit = true;
    }
    if (bottomZombiePos < this->blockDown) {
        zombie->setPosition(Point(zombie->getPosition().x, this->blockDown + zombieBody->getHeight() / 2));
        isTouchLimit = true;
    }
    
    // calculate vector to man
    Point vectorToMan = Point(manBody->getPosition() - zombieBody->getPosition());
    Point vectorToMove = vectorToMan;
    
    // assign zombie attack left or right of man
    bool attackLeft = true;
    for (int i = 0; i < this->zombies->size(); i++) {
        Zombie* zombieObj = this->zombies->at(i);
        if (zombieObj->getObjId() == zombie->getObjId()) {
            if (i % 2 == 0) {
                attackLeft = !attackLeft;
                break;
            }
        }
    }

    // check interact between attack range of zombie and protected range of man
    // first, create attack range
    RectBody* attackRange = zombie->getRangeAttack();
    if (zombie->getBody()->isFlipX()) {
        attackRange->setPosition(zombie->getPosition() - Point(attackRange->getWidth() / 2, 0));
    } else {
        attackRange->setPosition(zombie->getPosition() + Point(attackRange->getWidth() / 2, 0));
    }
    
    // then, protected range
    RectBody* protectedRange = new RectBody(manBody->getWidth() * 2, manBody->getHeight() * 2);
    if (attackLeft) {
        protectedRange->setPosition(manBody->getPosition() - Point(protectedRange->getWidth() / 2, 0));
    } else {
        protectedRange->setPosition(manBody->getPosition() + Point(protectedRange->getWidth() / 2, 0));
    }
    
    // when distance betwwen zombie and man more than 100, zombie will go left or right only, neither go up nor go down
    if (abs(vectorToMove.x) > 100) {
        vectorToMove.y = 0;
    }
    
    //DONE DONE DONE
    
    // protected range and attack range collide, zombie will
    // go to correct attack side
    // if touch limit, move as normal
    if (Utils::b2Interact(protectedRange, attackRange) &&
        !isTouchLimit &&
        !zombie->isFireType()) {
        
        float minX = protectedRange->getPosition().x - protectedRange->getWidth() / 2 - attackRange->getWidth() / 2;
        float maxX = protectedRange->getPosition().x + protectedRange->getWidth() / 2 + attackRange->getWidth() / 2;
        float minY = protectedRange->getPosition().y - protectedRange->getHeight() / 2 - attackRange->getHeight() / 2;
        float maxY = protectedRange->getPosition().y + protectedRange->getHeight() / 2 + attackRange->getHeight() / 2;
        
        float touchLeft = abs(attackRange->getPosition().x - minX);
        float touchRight = abs(attackRange->getPosition().x - maxX);
        float touchUp = abs(attackRange->getPosition().y - maxY);
        float touchDown = abs(attackRange->getPosition().y - minY);
        
        if (touchLeft < touchRight &&
            touchLeft < touchUp &&
            touchLeft < touchDown) {
            
            if (attackLeft) {
                vectorToMove.x = 0;
                vectorToMove.y = vectorToMove.y - 100;
            }
        }
        
        if (touchRight < touchUp &&
            touchRight < touchLeft &&
            touchRight < touchDown) {
            
            if (!attackLeft) {
                vectorToMove.x = 0;
                vectorToMove.y = vectorToMove.y - 100;
            }
        }
        
        if (touchUp < touchRight &&
            touchUp < touchLeft &&
            touchUp < touchDown) {
            
            if (!attackLeft) {
                vectorToMove.x = vectorToMove.x - 100;
                vectorToMove.y = 0;
            } else {
                vectorToMove.x = vectorToMove.x + 100;
                vectorToMove.y = 0;
            }
        }
        
        if (touchDown < touchRight &&
            touchDown < touchLeft &&
            touchDown < touchUp) {
            
            if (!attackLeft) {
                vectorToMove.x = vectorToMove.x - 100;
                vectorToMove.y = 0;
            } else {
                vectorToMove.x = vectorToMove.x + 100;
                vectorToMove.y = 0;
            }
        }
    }
    
    if (zombie->isFireType()) {
        bool zombieLeftMan = vectorToMan.x > 0;
        float angle = 45 * (3.14f / 180);
        if (vectorToMan.y != 0) {
            float newVectorX = abs(vectorToMan.y / cos(angle));
            vectorToMove.x = zombieLeftMan ? newVectorX : -newVectorX;
            vectorToMove.y = vectorToMan.y;
        }
    }
    
    // determite zombie velocity based on vector to move calculated
    float angle = atan2(vectorToMove.x, vectorToMove.y);
    float velocityX = zombie->getRunVelocity() * sin(angle);
    float velocityY = zombie->getRunVelocity() * cos(angle);
    zombie->getBody()->setVelocity(Point(velocityX, velocityY));
    
    // update time to being hurt
    // If zombie being knock out, zombie will fall and getup if zombie still alive
    // else zombie just run animation show he being hit
    bool isHurt = false;
    if (zombie->getCurrentDelayHurting() > 0) {
        isHurt = true;
        zombie->getBody()->setVelocity(Point::ZERO);
        
        if (zombie->isKnockOut()) {
            zombieState = (char*) STATE_KO;
            
            if (zombie->getFallPositions()->size() > 0) {
                ThePoint* point = zombie->getFallPositions()->at(0);
                Face* faceChar = zombie->getFace((char*) FACE_CHAR);
                
                if (zombie->getCurrentVectorY() < point->getY()) {
                    float newVectorY = zombie->getCurrentVectorY() + CONF_FALL_VELOCITY_Y * dt;
                    if (newVectorY > point->getY()) {
                        newVectorY = point->getY();
                    }
                    zombie->setCurrentVectorY(newVectorY);
                    faceChar->setOOVector(Point(0, newVectorY));
                }
                if (zombie->getCurrentVectorY() > point->getY()) {
                    float newVectorY = zombie->getCurrentVectorY() - CONF_FALL_VELOCITY_Y * dt;
                    if (newVectorY < point->getY()) {
                        newVectorY = point->getY();
                    }
                    zombie->setCurrentVectorY(newVectorY);
                    faceChar->setOOVector(Point(0, newVectorY));
                }
                if (zombie->getPosition().x < point->getX()) {
                    float newX = zombie->getPosition().x + CONF_FALL_VELOCITY_X * dt;
                    if (newX > point->getX()) {
                        newX = point->getX();
                    }
                    zombie->setPosition(Point(newX, zombie->getPosition().y));
                }
                if (zombie->getPosition().x > point->getX()) {
                    float newX = zombie->getPosition().x - CONF_FALL_VELOCITY_X * dt;
                    if (newX < point->getX()) {
                        newX = point->getX();
                    }
                    zombie->setPosition(Point(newX, zombie->getPosition().y));
                }
                
                if (point->getX() == zombie->getPosition().x && point->getY() == zombie->getCurrentVectorY()) {
                    zombie->getFallPositions()->erase(0);
                }
            } else {
                if (!zombie->isDead()) {
                    float delayToHurting = zombie->getCurrentDelayHurting();
                    zombie->setCurrentDelayHurting(delayToHurting - dt);
                    
                    float delayGetUp = zombie->getCurrentDelayGetUp();
                    zombie->setCurrentDelayGetUp(delayGetUp - dt);
                    if (delayGetUp < 0) {
                        zombieState = (char*) STATE_GET_UP;
                    }
                }
            }
        } else {
            float delayToHurting = zombie->getCurrentDelayHurting();
            zombie->setCurrentDelayHurting(delayToHurting - dt);
            
            if (zombie->isHurtUp()) {
                zombieState = (char*) STATE_HIT_HIGH;
            } else {
                zombieState = (char*) STATE_HIT_LOW;
            }
        }
        if (zombie->getCurrentDelayHurting() < 0) {
            zombie->setKnockOut(false);
        }
    }
    
    // check if zombie not being hit
    if (!isHurt) {
        // reset flip follow vector to man calculated, if not defending
        if (zombie->getCurrentDelayDefending() <= 0) {
            if (vectorToMan.x > 0) {
                zombie->getBody()->setFlipX(false);
            }
            if (vectorToMan.x < 0) {
                zombie->getBody()->setFlipX(true);
            }
        }
        
        // update time zombie defending
        // update time zombie punching
        // update time zombie wating to next attack
        // check if zombie will run or will attack man
        if (zombie->getCurrentDelayDefending() > 0) {
            float delayToDefending = zombie->getCurrentDelayDefending();
            zombie->setCurrentDelayDefending(delayToDefending - dt);
            if (zombie->getBody()->isFlipX()) {
                zombie->getBody()->setVelocity(Point(100, 0));
            } else {
                zombie->getBody()->setVelocity(Point(-100, 0));
            }
            
            zombieState = (char*) STATE_BLOCK;
        } else if (zombie->getCurrentDelayPunching() > 0) {
            float delayToPunching = zombie->getCurrentDelayPunching();
            zombie->setCurrentDelayPunching(delayToPunching - dt);
            zombie->getBody()->setVelocity(Point::ZERO);
            zombieState = (char*) STATE_ATTACK;
            
            if (zombie->isFireType()) {
                if (zombie->getCurrentDelayPunching() < 0) {
                    zombie->getBody()->setVelocity(Point::ZERO);
                    zombieState = (char*) STATE_IDLE;
                }
            } else {
                if (zombie->getCurrentDelayPunching() < zombie->getHalfPunchingTime() &&
                    zombie->getHalfPunchingTime() != 0) {
                    this->registerDamage(attackRange, zombie, DAMAGE_ZOMBIE);
                    zombie->setHalfPunchingTime(0);
                }
                
                if (zombie->getCurrentDelayPunching() < 0) {
                    zombie->setCurrentDelayAfterPunch(CONF_DELAY_ZOMBIE_ATTACK);
                }
            }
        } else if (zombie->getCurrentDelayAfterPunch() > 0) {
            float delayAfterPunch = zombie->getCurrentDelayAfterPunch();
            zombie->setCurrentDelayAfterPunch(delayAfterPunch - dt);
            
            if (Utils::b2Interact(attackRange, manBody) &&
                this->man->getCurrentVectorY() == CONF_MAN_MIN_VECTOR_Y) {
                
                zombie->getBody()->setVelocity(Point::ZERO);
                zombieState = (char*) STATE_IDLE;
            } else {
                zombieState = (char*) STATE_RUN;
            }
        } else {
            if (abs(vectorToMan.y) < zombie->getAttackableY() &&
                abs(vectorToMan.x) < zombie->getAttackableX()) {
                
                zombie->getBody()->setVelocity(Point::ZERO);
                
                if (zombie->isFireType()) {
                    zombie->setAiming(true);
                } else {
                    zombie->punch();
                    zombieState = (char*) STATE_ATTACK;
                }
            } else {
                zombieState = (char*) STATE_RUN;
            }
            
            // check if zombie aiming
            if (zombie->isAiming()) {
                zombie->getBody()->setVelocity(Point::ZERO);
                zombieState = (char*) STATE_IDLE;
                
                if (zombie->aim(dt)) {
                    zombie->punch();
                    zombieState = (char*) STATE_ATTACK;
                    
                    TheShot* shot = new TheShot(SHOT_TYPE_LASER, zombie->getPosition());
                    shot->initFaces(KEY_FX, gameModel->getAnimations());
                    shot->setState((char*) STATE_EYE_LASER, (char*) FACE_CHAR);
                    shot->getBody()->setFlipX(zombie->getBody()->isFlipX());
                    shot->setPosition(zombie->getBody()->getPosition());
                    if (shot->getBody()->isFlipX()) {
                        shot->getBody()->setVelocity(Point(-300, 0));
                    } else {
                        shot->getBody()->setVelocity(Point(300, 0));
                    }
                    this->shots->pushBack(shot);
                    
                    DataEvent* dataEvent = new DataEvent();
                    dataEvent->setEventCode(EVT_NEW_SHOT);
                    dataEvent->setArgumentObject(shot);
                    CC_SAFE_RETAIN(shot);
                    gameModel->fireEvent(dataEvent);
                    zombie->setAiming(false);
                }
            }
            
            // driving
            if (zombie->isDriveType()) {
                zombie->getBody()->setVelocity(Point(-150, 0));
                zombie->getBody()->setFlipX(true);
                zombieState = (char*) STATE_DRIVE;
                
                RectBody* attackRange = zombie->getRangeAttack();
                attackRange->setPosition(zombie->getPosition() - Point(attackRange->getWidth() / 2, 0));
                if (Utils::b2Interact(attackRange, this->man->getBody())) {
                    this->registerDamage(attackRange, zombie, DAMAGE_ZOMBIE);
                }
            }
        }
    }
    
    // update shadow's scale
    zombie->updateShadow();
    
    // update zombie animation
    zombie->setState(zombieState, (char*) FACE_CHAR);
}

void Level::loadMap() {
    GameModel* gameModel = GameModel::getInstance();
    int chapterIndex = gameModel->getMapData()->getChapter(this->level);
    int chapterLevelIndex = gameModel->getMapData()->getChapterLevel(this->level);
    
    char mapPath[200];
    sprintf(mapPath, DOC_LEVEL, chapterIndex, chapterLevelIndex);

    Json* root;
    string fileData = Utils::readFile(mapPath, false, true, false);
    root = Json_create(fileData.data());
    
    if (root) {
        Json* level = Json_getItem(root, "level");
        Json* barrels = Json_getItem(level, "barrels");
        Json* stops = Json_getItem(level, "stops");
        Json* spawn_points = Json_getItem(level, "spawn_points");
        Json* elevators = Json_getItem(level, "elevators");
//        Json* traps = Json_getItem(level, "traps");
//        Json* bosses = Json_getItem(level, "bosses");
        Json* walls = Json_getItem(level, "walls");
        
        if (barrels != NULL) {
            int size = barrels->size;
            for (int i = 0; i < size; i++) {
                Json* item = Json_getItemAt(barrels, i);
                float x = atof(Json_getString(item, "-x", ""));
                float y = atof(Json_getString(item, "-y", ""));
                int type = atoi(Json_getString(item, "-type", ""));
                
                Barrel* newItem = new Barrel(type, Point(x, y));
                this->barrels->pushBack(newItem);
            }
        }
        
        if (stops != NULL) {
            int size = stops->size;
            for (int i = 0; i < size; i++) {
                Json* item = Json_getItemAt(stops, i);
                this->stops->pushBack(__Float::create(atof(item->valueString)));
            }
        }
        
        if (spawn_points != NULL) {
            int size = spawn_points->size;
            for (int i = 0; i < size; i++) {
                Json* item = Json_getItemAt(spawn_points, i);
                float x = atof(Json_getString(item, "-x", ""));
                float y = atof(Json_getString(item, "-y", ""));
                int type = atoi(Json_getString(item, "-type", ""));
                
                SpawnPoint* newItem = new SpawnPoint(type, Point(x, y));
                this->spawnPoints->pushBack(newItem);
            }
        }
        
        if (elevators != NULL) {
            int size = elevators->size;
            for (int i = 0; i < size; i++) {
                Json* item = Json_getItemAt(elevators, i);
                float x = atof(Json_getString(item, "-x", ""));
                float y = atof(Json_getString(item, "-y", ""));
                int type = atoi(Json_getString(item, "-type", ""));
                
                Elevator* newItem = new Elevator(type, Point(x, y));
                this->elevators->pushBack(newItem);
            }
        }
        
        if (walls != NULL) {
            int size = walls->size;
            for (int i = 0; i < size; i++) {
                Json* item = Json_getItemAt(walls, i);
                float x = atof(Json_getString(item, "-x", ""));
                float y = atof(Json_getString(item, "-y", ""));
                int type = atoi(Json_getString(item, "-type", ""));
                
                Wall* newItem = new Wall(type, Point(x, y));
                this->walls->pushBack(newItem);
            }
        }
    }
    
    this->man = new Man();
    this->man->initFaces(KEY_PLAYER, gameModel->getAnimations());
    this->man->setPosition(Point(CONF_MAN_OFF_SET_X, (CONF_MAX_Y - CONF_MIN_Y) / 2 + CONF_MIN_Y));
    this->man->getBody()->setVelocity(Point(CONF_MAN_APPEAR_VELOCITY, 0));
    CC_SAFE_RETAIN(this->man);
    
    if (this->stops->size() > 0) {
        this->blockLeft = 0;
        this->blockRight = this->stops->at(this->currentCheckPoint)->getValue();
        this->blockUp = CONF_MAX_Y;
        this->blockDown = CONF_MIN_Y;
        
        // init position
        float realWidth = this->blockRight - this->blockLeft;
        float darkX = realWidth - gameModel->getDisplayResolutionSize().width;
        
        //float distanceWidth = this->blockRight - this->blockLeft;
        float distanceToFollowX = (realWidth - darkX) / 2;
        
        RectBody* manBody = (RectBody*) this->man->getBody();
        if (distanceToFollowX > 0) {
            float distanceMinX = manBody->getPosition().x - this->blockLeft - distanceToFollowX;
            if (distanceMinX > 0) {
                float distanceMaxX = this->blockRight - manBody->getPosition().x;
                if (distanceMaxX > distanceToFollowX) {
                    this->airX = distanceMinX;
                }
            }
        }
        
        float maxDarkX = this->blockRight - distanceToFollowX;
        if (manBody->getPosition().x >= maxDarkX) {
            this->airX = maxDarkX - this->blockLeft - distanceToFollowX;
        }
    }

    this->callEnemies();
}

void Level::nextCheckPoint() {
    this->currentCheckPoint++;
    if (this->currentCheckPoint >= this->stops->size()) {
        this->currentCheckPoint = this->stops->size() - 1;
    }
    this->blockRight = this->stops->at(this->currentCheckPoint)->getValue();
    
    // update camera target
    GameModel* gameModel = GameModel::getInstance();
    RectBody* manBody = (RectBody*) this->man->getBody();
    float realWidth = this->blockRight - this->blockLeft;
    float darkX = realWidth - gameModel->getDisplayResolutionSize().width;
    
    float distanceWidth = this->blockRight - this->blockLeft;
    float distanceToFollowX = (distanceWidth - darkX) / 2;
    
    if (distanceToFollowX > 0) {
        float distanceMinX = manBody->getPosition().x - this->blockLeft - distanceToFollowX;
        if (distanceMinX > 0) {
            float distanceMaxX = this->blockRight - manBody->getPosition().x;
            if (distanceMaxX > distanceToFollowX) {
                if (distanceMinX > this->airX) {
                    this->cameraTarget = distanceMinX;
                }
            }
        }
    }
}

void Level::registerDamage(RectBody* damage, AnimatedObject* owner, int damageType) {
    GameModel* gameModel = GameModel::getInstance();
    if (damageType == DAMAGE_ZOMBIE) {
        if (Utils::b2Interact(damage, this->man->getBody()) &&
            this->man->getCurrentVectorY() == CONF_MAN_MIN_VECTOR_Y &&
            !this->man->isKnockOut() &&
            this->man->getCurrentDelayHurting() <= 0) {
            
            Zombie* enemyOwner = (Zombie*) owner;
            float strength = enemyOwner->getStrength();
            float vit = this->man->getVitality();
            float damagePoint = strength - vit;
            if (damagePoint < 0) {
                damagePoint = 0;
            }
            
            float manHP = this->man->getHP();
            float newManHP = manHP - damagePoint;
            if (newManHP < 0) {
                newManHP = 0;
            }
            this->man->setHP(newManHP);
            
            // down angry
            if (!this->man->isTransformer()) {
                float newAP = this->man->getAP() - (damagePoint * CONF_DOWN_ANGRY);
                if (newAP < 0) {
                    newAP = 0;
                }
                this->man->setAP(newAP);
            }
            
            if (this->man->getHP() == 0) {
                this->man->ko();
            } else {
                DataEvent* dataEvent = new DataEvent();
                dataEvent->setEventCode(EVT_NEW_HIT_PUNCH);
                dataEvent->setArgumentPoint(this->man->getPosition());
                dataEvent->setArgumentReference(this->man);
                gameModel->fireEvent(dataEvent);
                this->man->hurt();
            }
        }
    } else if (damageType == DAMAGE_MAN) {
        __Array* zombieToDelete = __Array::create();
        
        for (int i = 0; i < this->zombies->size(); i++) {
            Zombie* zombie = this->zombies->at(i);
            if (Utils::b2Interact(damage, zombie->getBody()) &&
                !zombie->isKnockOut() &&
                zombie->getCurrentDelayHurting() <= 0 &&
                !zombie->isDriveType()) {
                
                if (zombie->isBlockType()) {
                    float random = Utils::random();
                    if (random < 0.3f) {
                        if (this->man->getPosition().x < zombie->getPosition().x &&
                            !this->man->getBody()->isFlipX() &&
                            zombie->getBody()->isFlipX()) {
                            
                            zombie->setCurrentDelayDefending(CONF_DELAY_ZOMBIE_DEFENDING);
                        }
                        
                        if (this->man->getPosition().x > zombie->getPosition().x &&
                            this->man->getBody()->isFlipX() &&
                            !zombie->getBody()->isFlipX()) {
                            
                            zombie->setCurrentDelayDefending(CONF_DELAY_ZOMBIE_DEFENDING);
                        }
                        continue;
                    }
                }
                float strength = this->man->getStrength();
                if (this->man->isTransformer()) {
                    strength = strength * CONF_TRANSFORMER_RATE;
                }
                float vit = zombie->getVitality();
                float damagePoint = strength - vit;
                if (damagePoint < 0) {
                    damagePoint = 0;
                }
                
                float zombieHP = zombie->getHP();
                float newZombieHP = zombieHP - damagePoint;
                if (newZombieHP < 0) {
                    newZombieHP = 0;
                }
                zombie->setHP(newZombieHP);
                
                // gain angry
                if (!this->man->isTransformer()) {
                    float newAP = this->man->getAP() + (damagePoint * CONF_UP_ANGRY);
                    if (newAP >= CONF_MAN_ANGRY) {
                        newAP = CONF_MAN_ANGRY;
                        this->man->setTransformer(true);
                        Face* charFace = this->man->getFace((char*) FACE_CHAR);
                        charFace->runAction(charFace->getState(STATE_FRENZY_ON));
                    }
                    this->man->setAP(newAP);
                }
                
                // gain score
                float gainedScore = CONF_MAN_PUNCH_SCORE;
                if (this->man->getCurrentPunch() == 5) {
                    gainedScore = gainedScore * 2;
                }
                this->setScore(this->getScore() + gainedScore);
                
                if (zombie->getHP() == 0) {
                    zombie->ko();
                    DataEvent* dataEvent = new DataEvent();
                    dataEvent->setEventCode(EVT_ZOMBIE_ALMOST_DIE);

                    Face* charFace = zombie->getFace((char*) FACE_CHAR);
                    ActionInterval* koAction = (ActionInterval*) charFace->getState(STATE_KO);
                    float koDuration = (koAction)->getDuration();
                    charFace->setState(koAction);
                    
                    dataEvent->setArgumentFloat(koDuration);
                    dataEvent->setArgumentFloat2(CONF_DELAY_TO_DISAPPEAR);
                    dataEvent->setArgumentObject(zombie);
                    CC_SAFE_RETAIN(zombie);
                    gameModel->fireEvent(dataEvent);
                    
                    zombieToDelete->addObject(zombie);
                } else {
                    DataEvent* dataEvent = new DataEvent();
                    dataEvent->setEventCode(EVT_NEW_HIT_PUNCH);
                    dataEvent->setArgumentPoint(zombie->getPosition());
                    dataEvent->setArgumentReference(zombie);
                    gameModel->fireEvent(dataEvent);
                    
                    if (this->man->getCurrentPunch() == 5) {
                        zombie->ko();
                        
                        float finalZombieX = zombie->getFallPositions()->at(zombie->getFallPositions()->size() - 1)->getX();
                        float finalZombieY = zombie->getPosition().y + CONF_STAR_VECTOR_Y;
                        if (zombie->getBody()->isFlipX()) {
                            finalZombieX += CONF_STAR_VECTOR_X;
                        } else {
                            finalZombieX -= CONF_STAR_VECTOR_X;
                        }
                        
                        DataEvent* dataEvent = new DataEvent();
                        dataEvent->setEventCode(EVT_STAR_ROTATE);
                        dataEvent->setArgumentPoint(Point(finalZombieX, finalZombieY));
                        
                        Face* charFace = zombie->getFace((char*) FACE_CHAR);
                        float koDuration = ((ActionInterval*) charFace->getState(STATE_KO))->getDuration();

                        dataEvent->setArgumentFloat(koDuration);
                        dataEvent->setArgumentFloat2(zombie->getCurrentDelayGetUp() - koDuration);
                        gameModel->fireEvent(dataEvent);
                    } else {
                        zombie->hurt();
                    }
                }
            }
        }
        
        for (int i = 0; i < zombieToDelete->count(); i++) {
            Zombie* z = (Zombie*) zombieToDelete->getObjectAtIndex(i);
            this->zombies->eraseObject(z);
        }
    } else if (damageType == DAMAGE_SHOT) {
        if (Utils::b2Interact(damage, this->man->getBody()) &&
            this->man->getCurrentVectorY() == CONF_MAN_MIN_VECTOR_Y &&
            !this->man->isKnockOut() &&
            this->man->getCurrentDelayHurting() <= 0) {
            
            TheShot* shot = (TheShot*) owner;
            float strength = shot->getStrength();
            float vit = this->man->getVitality();
            float damagePoint = strength - vit;
            if (damagePoint < 0) {
                damagePoint = 0;
            }
            
            float manHP = this->man->getHP();
            float newManHP = manHP - damagePoint;
            if (newManHP < 0) {
                newManHP = 0;
            }
            this->man->setHP(newManHP);
            
            // down angry
            if (!this->man->isTransformer()) {
                float newAP = this->man->getAP() - (damagePoint * CONF_DOWN_ANGRY);
                if (newAP < 0) {
                    newAP = 0;
                }
                this->man->setAP(newAP);
            }
            
            if (this->man->getHP() == 0) {
                this->man->ko();
            } else {
                DataEvent* dataEvent = new DataEvent();
                dataEvent->setEventCode(EVT_NEW_HIT_PUNCH);
                dataEvent->setArgumentPoint(this->man->getPosition());
                dataEvent->setArgumentReference(this->man);
                gameModel->fireEvent(dataEvent);
                this->man->hurt();
            }
            
            DataEvent* dataEvent = new DataEvent();
            dataEvent->setEventCode(EVT_DELETE_SHOT);
            dataEvent->setArgumentObject(shot);
            CC_SAFE_RETAIN(shot);
            gameModel->fireEvent(dataEvent);
            this->shots->eraseObject(shot);
        }
    }
}

void Level::callEnemies() {
    GameModel* gameModel = GameModel::getInstance();
    for (int i = 0; i < this->spawnPoints->size(); i++) {
        SpawnPoint* spawnPoint = this->spawnPoints->at(i);
        Vector<Zombie*>* enemyInfos = new Vector<Zombie*>();

        enemyInfos->pushBack(new Zombie(KEY_WORKER, Point(200, 100)));
//        if (spawnPoint->getType() == 1) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        } else if (spawnPoint->getType() == 2) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        } else if (spawnPoint->getType() == 3) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        } else if (spawnPoint->getType() == 4) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        } else if (spawnPoint->getType() == 5) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        } else if (spawnPoint->getType() == 6) {
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//            enemyInfos->pushBack(new Zombie(KEY_ZOMBIE, Point(200, 100)));
//        }
        
        for (int j = 0; j < enemyInfos->size(); j++) {
            Zombie* enemyInfo = enemyInfos->at(j);
            
            Zombie* zombie = new Zombie(enemyInfo->getType());
            zombie->initFaces(zombie->getType(), gameModel->getAnimations());
            zombie->setPosition(enemyInfo->getPos() + spawnPoint->getPos());
            zombie->setCheckPoint(i);
            zombie->getBody()->setVisible(false);
            this->zombies->pushBack(zombie);
        }
    }
}

void Level::callEnemiesAtCheckPoint() {
    for (int i = 0; i < this->zombies->size(); i++) {
        Zombie* zombie = this->zombies->at(i);
        if (zombie->getCheckPoint() == this->currentCheckPoint) {
            zombie->setCalled(true);
            for (int j = 0; j < zombie->getFaces()->count(); j++) {
                Face* face = (Face*) zombie->getFaces()->getObjectAtIndex(j);
                face->setVisible(true);
            }
        }
    }
}

void Level::createEndzone(Size contentSize) {
    __Float* lastStop = this->stops->at(this->stops->size() - 1);
    this->endZoneBody = new RectBody(contentSize.width, contentSize.height);
    this->endZoneBody->setPosition(Point(lastStop->getValue() - contentSize.width / 2, CONF_END_ZONE_Y));
    CC_SAFE_RETAIN(this->endZoneBody);
}
