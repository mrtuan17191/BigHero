#include "UGLayer.h"

void UGLayer::onEnter() {
    Layer::onEnter();
}

void UGLayer::onExit() {
    Layer::onExit();
    this->removeAllChildrenWithCleanup(true);
}