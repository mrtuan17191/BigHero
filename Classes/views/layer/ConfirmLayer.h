#ifndef CONFIRM_LAYER_H
#define CONFIRM_LAYER_H

#include "AbstractLayer.h"

USING_NS_CC;

class ConfirmLayer : public AbstractLayer {
    UG_PROPERTY_GETTER(Face*, backgroundFade, BackgroundFade);
    UG_PROPERTY_GETTER(Face*, menuFace, MenuFace);
    UG_PROPERTY_GETTER(MenuItemSprite*, okButton, OkButton);
    UG_PROPERTY_GETTER(MenuItemSprite*, cancelButton, CancelButton);
    
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
    
    void onOptionBoardMoveDownDone();
    void onOptionBoardMoveUpDone();
    
    void enableParentButton(bool enabled);
    void enableButton(bool enabled);
    
    void onOkButtonClick(Ref* sender);
    void onCancelButtonClick(Ref* sender);
public:
    ConfirmLayer(View* parent);
    ~ConfirmLayer();
    void show();
    void hide();
    
    void update(float dt);
};

#endif