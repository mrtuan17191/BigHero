#include "MapData.h"

MapData::MapData() {
    this->autorelease();
    this->levels = new Vector<__Dictionary*>();
}

MapData::~MapData() {
    CC_SAFE_DELETE(this->levels);
}

int MapData::getLevelSize() {
    return (int) this->levels->size();
}

int MapData::getChapter(int level) {
    int chapterIndex = ((level - 1) / CONF_LEVELS_PER_CHAPTER) + 1;
    return chapterIndex;
}

int MapData::getChapterLevel(int level) {
    int levelIndex = ((level - 1) % CONF_LEVELS_PER_CHAPTER) + 1;
    return levelIndex;
}

std::string MapData::getLevelName(int level) {
    __Dictionary* levelDict = this->levels->at(level - 1);
    __String* levelName = (__String*) levelDict->objectForKey("name");
    return levelName->getCString();
}

bool MapData::getLevelRequireSocial(int level) {
    __Dictionary* levelDict = this->levels->at(level - 1);
    __Bool* requireSocial = (__Bool*) levelDict->objectForKey("requireSocial");
    return requireSocial->getValue();
}