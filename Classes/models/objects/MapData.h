#ifndef MAP_DATA_H
#define MAP_DATA_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

class MapData : public Ref {
    UG_PROPERTY_GETTER(Vector<__Dictionary*>*, levels, Levels);
protected:
public:
    MapData();
    virtual ~MapData();
    
    int getLevelSize();
    int getChapter(int level);
    int getChapterLevel(int level);
    std::string getLevelName(int level);
    bool getLevelRequireSocial(int level);
};

#endif