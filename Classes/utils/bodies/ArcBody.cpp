#include "ArcBody.h"

ArcBody::ArcBody(float radius, float angle) {
    this->radius = radius;
    this->angle = angle;
    this->setRect(false);
}

ArcBody::~ArcBody() {   
}

bool ArcBody::contains(Point point) {
    return this->containsInDistance(point, 1);
}

bool ArcBody::containsInDistance(Point point, float distanceRate) {
    float offX = point.x - this->position.x;
    float offY = point.y - this->position.y;
    
    float distance = sqrt(offX * offX + offY * offY);

    if (distance < radius * this->getScale() * distanceRate) {
        if (this->angle < 360) {
            float targetAngle = offX != 0 ? atan(offY / offX) * 180 / 3.14f : 90;
            float angle1 = offX >= 0 ? targetAngle : targetAngle + 180;
            
            float limitAngle1 = this->rotation + this->angle / 2 * this->getScale();
            float limitAngle2 = this->rotation - this->angle / 2 * this->getScale();
            
            if (angle1 > limitAngle2 && angle1 < limitAngle1) {
                return true;
            }
        } else {
            return true;
        }
    }
    return false;
}

bool ArcBody::nearby(Point point) {
    float offX = point.x - this->position.x;
    float offY =  point.y - this->position.y;
    
    float distance = sqrt(offX * offX + offY * offY);
    
    if (distance < (this->radius * 120 / 100) * this->getScale()) {
        if (this->angle == 360) {
            return true;
        }
    }
    return false;
}