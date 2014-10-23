#include "UGMenu.h"

void UGMenu::onEnter() {
    Menu::onEnter();
}

void UGMenu::onExit() {
    Menu::onExit();
    this->removeAllChildrenWithCleanup(true);
}