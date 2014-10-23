#include "UGLayerColor.h"

void UGLayerColor::onEnter() {
    CCLayer::onEnter();
}

void UGLayerColor::onExit() {
    CCLayer::onExit();
    this->removeAllChildrenWithCleanup(true);
}

bool UGLayerColor::initWithColor(const Color4B& color) {
    return LayerColor::initWithColor(color);
}
