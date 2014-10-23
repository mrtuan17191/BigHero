#ifndef OPTION_LAYER_H
#define OPTION_LAYER_H

#include "AbstractLayer.h"

USING_NS_CC;

class OptionLayer : public AbstractLayer {
    UG_PROPERTY_GETTER(Face*, backgroundFade, BackgroundFade);
    UG_PROPERTY_GETTER(Face*, menuFace, MenuFace);
    UG_PROPERTY_GETTER(MenuItemSprite*, musicButton, MusicButton);
    UG_PROPERTY_GETTER(MenuItemSprite*, sfxButton, SfxButton);
    UG_PROPERTY_GETTER(MenuItemSprite*, closeButton, CloseButton);
    
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
    
    void onMusicButtonClick(Ref* sender);
    void onSfxButtonClick(Ref* sender);
    void onCloseButtonClick(Ref* sender);
public:
    OptionLayer(View* parent);
    ~OptionLayer();
    void show();
    void hide();
    
    void update(float dt);
};

#endif