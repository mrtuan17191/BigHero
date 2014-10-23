#ifndef SHARE_LAYER_H
#define SHARE_LAYER_H

#include "AbstractLayer.h"

USING_NS_CC;

class ShareLayer : public AbstractLayer {
    UG_PROPERTY_GETTER(Face*, backgroundFade, BackgroundFade);
    UG_PROPERTY_GETTER(Face*, menuFace, MenuFace);
    UG_PROPERTY_GETTER(MenuItemSprite*, okButton, OkButton);
    UG_PROPERTY_GETTER(MenuItemSprite*, shareFacebookButton, ShareFacebookButton);
    UG_PROPERTY_GETTER(MenuItemSprite*, shareTwitterButton, ShareTwitterButton);
    
    UG_PROPERTY_GETTER(Vector<MenuItemSprite*>*, parentButtons, ParentButtons);
protected:
    View* parent;
    UGLayer* mainLayer;
private:
    void initialize();
    void createLayers();
    void createLayersContent();
    
    virtual void onEnter();
    virtual void onExit();
    
    void onShareBoardMoveDownDone();
    void onShareBoardMoveUpDone();
    
    void enableParentButton(bool enabled);
    void enableButton(bool enabled);
    
    void onOkButtonClick(Ref* sender);
    void onShareFacebookClick(Ref* sender);
    void onShareTwitterClick(Ref* sender);
public:
    ShareLayer(View* parent);
    ~ShareLayer();
    void show(std::string message);
    void hide();
    
    void update(float dt);
};

#endif