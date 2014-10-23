#ifndef CONTROLLER_LAYER_H
#define CONTROLLER_LAYER_H

#include "AbstractLayer.h"

class ControllerLayer : public AbstractLayer {
protected:
    EventListenerTouchOneByOne* touchListener;
    Point kCenter;
    Face* thumb;
    bool isPressed;
    Point velocity;
    View* parent;
public:
	ControllerLayer(View* parent);
	virtual ~ControllerLayer();
    
    void initialize();
    void createLayers();
    void createLayersContent();

    virtual void resetJoystick();
    virtual bool handleLastTouch();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    void updateVelocity(Point point);
    
    void stopJoystick();
    Point getVelocity();
    float getAngle();
};

#endif /* ControllerLayer_H_ */
