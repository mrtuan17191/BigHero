#ifndef ARC_BODY_H
#define ARC_BODY_H

#include "Body.h"

class ArcBody : public Body {
protected:
    UG_PROPERTY_GETTER(float, radius, Radius);
    UG_PROPERTY_GETTER(float, angle, Angle);
private:
public:
    ArcBody(float radius, float angle);
    ~ArcBody();
    virtual bool contains(Point point);
    bool containsInDistance(Point point, float distanceRate);
    virtual bool nearby(Point point);
};

#endif