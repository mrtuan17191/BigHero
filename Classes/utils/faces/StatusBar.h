#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class StatusBar : public Sprite {
	CC_PROPERTY(float, value, Value);
    UG_PROPERTY_FULL(Sprite*, contentFace, ContentFace);
protected:
    cocos2d::Size contentFaceSize;
private:
	~StatusBar();
    virtual bool init();
public:
    StatusBar();
    void initWithParameter(char* barFrameName, char * contentFrameName);
};

#endif