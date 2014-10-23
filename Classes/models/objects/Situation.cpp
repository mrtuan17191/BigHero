#include "Situation.h"

Situation::Situation() {
    this->autorelease();
    this->subSituation = NULL;
    this->data = __Dictionary::create();
    CC_SAFE_RETAIN(this->data);
    this->target = NULL;
    this->situationType = -1;
}

Situation::~Situation() {
    CC_SAFE_RELEASE(this->data);
    CC_SAFE_RELEASE(this->subSituation);
    this->subSituation = NULL;
}

void Situation::raiseSituation(Situation* situation) {
    this->subSituation = situation;
    CC_SAFE_RETAIN(this->subSituation);
}

void Situation::endSubSituation() {
    if (this->subSituation != NULL) {
        CC_SAFE_RELEASE(this->subSituation);
        this->subSituation = NULL;
    }
}