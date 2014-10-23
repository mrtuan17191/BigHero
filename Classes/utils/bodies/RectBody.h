#ifndef RECT_BODY_H
#define RECT_BODY_H

#include "Body.h"

class RectBody : public Body {
protected:
    UG_PROPERTY_FULL(__Dictionary*, properties, Properties);
protected:
    float width;
    float height;
public:
    RectBody(float width, float height);
    ~RectBody();
    virtual bool contains(Point point);
    bool containsInDistance(Point point, float distanceRate);
    float getWidth();
    float getHeight();
    void setWidth(float width);
    void setHeight(float height);
};

#endif
