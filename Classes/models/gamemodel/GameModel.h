#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include "cocos2d.h"
#include "../objects/Level.h"
#include "../objects/SavedData.h"
#include "../objects/MapData.h"
#include "../../mvc/include/mvc.h"
#include "../../utils/include/utils.h"
#include "../../events/DataEvent.h"
#include "SoundModel.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "../../utils/AppUtils.h"
#endif

USING_NS_CC;

class Level;
class GameModel : public Model {
    UG_PROPERTY_FULL(Size, displayResolutionSize, DisplayResolutionSize);
    UG_PROPERTY_FULL(Point, vectorMove, VectorMove);
    UG_PROPERTY_FULL(Level*, currentLevel, CurrentLevel);
    UG_PROPERTY_FULL(SavedData*, savedData, SavedData);
    UG_PROPERTY_FULL(MapData*, mapData, MapData);
    UG_PROPERTY_FULL(int, gameState, GameState);
    UG_PROPERTY_FULL(int, level, Level);
    UG_PROPERTY_FULL(__Array*, animations, Animations);
protected:
    float currentDelayToNextMap;
    float currentDelayToTransit;
private:
    GameModel();
	~GameModel();
    
    void loadData();
    void loadAnimations();
public:
    static GameModel* getInstance();
    void play(int level);
    void next();
    bool isLastMap();
    void quit();
    void pause();
    void resume();
    void replay();
    void transit(float time);
    void update(float dt);
    void save();
    void updateLevel();
    
    bool checkLevelLock(int level);
    bool checkLevelRequireSocial(int level);
};
#endif