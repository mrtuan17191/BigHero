#ifndef BODY_H
#define BODY_H

#include "cocos2d.h"
#include "../UGCustomMacros.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class Body : public Ref {
    UG_PROPERTY_FULL_BOOL(rect, Rect);
    UG_PROPERTY_FULL(Point, position, Position);
    UG_PROPERTY_FULL(Point, velocity, Velocity);
    UG_PROPERTY_FULL(float, rotationVelocity, RotationVelocity);
    UG_PROPERTY_FULL_BOOL(physical, Physical);
    UG_PROPERTY_FULL_BOOL(visible, Visible);
    UG_PROPERTY_FULL(int, priority, Priority);
    UG_PROPERTY_FULL(float, rotation, Rotation);
    UG_PROPERTY_GETTER(float, scaleX, ScaleX);
    UG_PROPERTY_GETTER(float, scaleY, ScaleY);
    UG_PROPERTY_FULL(Point, anchorPoint, AnchorPoint);
	UG_PROPERTY_FULL(float, rotationSkewY, RotationSkewY);
	UG_PROPERTY_FULL(float, rotationSkewX, RotationSkewX);
    UG_PROPERTY_FULL_BOOL(flipX, FlipX);
    UG_PROPERTY_FULL_BOOL(flipY, FlipY);
	UG_PROPERTY_FULL(b2Body*, box2DBody, Box2DBody);
    UG_PROPERTY_FULL_BOOL(freeze, Freeze);
private:
    void initialize();
public:
    Body();
    ~Body();
    void setScale(float scale);
    float getScale();
    Point getNextPosition(float dt);
    float getNextRotation(float dt);
    void update(float dt);
    bool contains(Point point);
    bool nearby(Point point);
};

#endif
