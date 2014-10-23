#include "Obj.h"

Obj::Obj() {
    this->autorelease();
    this->objId = (int) (Utils::random() * 999999999);
    this->faces = CCArray::create();
    CC_SAFE_RETAIN(this->faces);
    this->ranges = CCArray::create();
    CC_SAFE_RETAIN(this->ranges);
    this->body = NULL;
}

Obj::~Obj() {
    Utils::safeRelease(this->faces);
    Utils::safeRelease(this->ranges);
    Utils::safeRelease(this->body);
}

void Obj::update(float dt) {    
    if (this->faces != NULL) {
        for (int i = 0; i < this->faces->count(); i++) {
            FaceBone* faceBone = dynamic_cast<FaceBone*>(this->faces->getObjectAtIndex(i));
            if (faceBone != NULL) {
                faceBone->setVisible(this->body->isVisible());
                faceBone->setPosition(this->body->getPosition());
                faceBone->setAnchorPoint(this->body->getAnchorPoint());
                faceBone->setRotation(-this->body->getRotation());
                faceBone->setScale(this->body->getScale());
            } else {
                Face* face = (Face*) this->faces->getObjectAtIndex(i);
                if (face != NULL) {
                    if (this->body != NULL) {
                        if (strcmp(face->getName(), FACE_MAIN) == 0) {
                            face->setVisible(this->body->isVisible());
                        }
                        face->setPosition(this->body->getPosition() + face->getOOVector());
                        face->setAnchorPoint(this->body->getAnchorPoint());
                        face->setRotation(-this->body->getRotation());
                        face->setScale(this->body->getScale());
                        face->setFlippedX(this->body->isFlipX());
                        
                        if (strcmp(face->getName(), FACE_SHADOW) == 0) {
                            face->setScale(this->body->getScale() * 0.25f);
                        }
                    }
                }
                
            }
        }
    }
    if (this->body != NULL) {
        this->body->update(dt);
    }
    
    if (this->ranges != NULL) {
        for (int i = 0; i < this->ranges->count(); i++) {
			Obj* o = (Obj*) this->ranges->getObjectAtIndex(i);
			if (o != NULL) {
				if (i == 0 && body != NULL) {
                    RectBody* radarBody = (RectBody*) body;
                    Point radarPos = radarBody->getPosition() - Point(0, radarBody->getHeight() / 2);
					o->getBody()->setPosition(radarPos);
				}
				o->update(dt);
			}
		}
    }
}

void Obj::setPosition(Point position) {
    this->body->setPosition(position);
}

Point Obj::getPosition() {
    return this->body->getPosition();
}

Face* Obj::getFace(char* faceName) {
    for (int i = 0; i < this->faces->count(); i++) {
        Face* face = (Face*) this->faces->getObjectAtIndex(i);
        if (strcmp(face->getName(), faceName) == 0) {
            return face;
        }
    }
    return NULL;
}

void Obj::removeFace(char* faceName) {
    for (int i = 0; i < this->faces->count(); i++) {
        Face* face = (Face*) this->faces->getObjectAtIndex(i);
        if (strcmp(face->getName(), faceName) == 0) {
            this->faces->removeObject(face);
            break;
        }
    }
}

void Obj::setState(char* stateName, char* faceName) {
    Face* theFace = this->getFace(faceName);
    if (theFace->getState(stateName) != NULL) {
        theFace->setState(theFace->getState(stateName));
    }
}