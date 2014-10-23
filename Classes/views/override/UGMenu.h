#ifndef UGMENU_H
#define UGMENU_H

#include "cocos2d.h"
USING_NS_CC;

class UGMenu : public Menu {
private:
    virtual void onEnter();
    virtual void onExit();
public:
    CREATE_FUNC(UGMenu);
};

#endif