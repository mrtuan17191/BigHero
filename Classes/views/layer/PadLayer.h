#ifndef PAD_LAYER_H
#define PAD_LAYER_H

#include "AbstractLayer.h"

class PadLayer : public AbstractLayer {
protected:
    EventListenerTouchOneByOne* touchListener;
    Face* padA;
    Face* padB;
    Touch* touchPadA;
    Touch* touchPadB;
    View* parent;
public:
	PadLayer(View* parent);
	virtual ~PadLayer();
    
    void initialize();
    void createLayers();
    void createLayersContent();

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
};

#endif /* PadLayer_H_ */
