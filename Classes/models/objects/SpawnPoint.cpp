#include "SpawnPoint.h"

SpawnPoint::SpawnPoint(int type, Point pos) {
    this->autorelease();
    this->type = type;
    this->pos = pos;
}

SpawnPoint::~SpawnPoint() {
}