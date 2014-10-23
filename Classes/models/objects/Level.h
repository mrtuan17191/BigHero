#ifndef LEVEL_H
#define LEVEL_H

#include "cocos2d.h"
#include "../gamemodel/GameModel.h"
#include "../objects/Zombie.h"
#include "../objects/Man.h"
#include "../objects/SpawnPoint.h"
#include "../objects/Wall.h"
#include "../objects/Elevator.h"
#include "../objects/Barrel.h"
#include "../objects/TheShot.h"
#include "../../utils/UGCustomMacros.h"
#include <cmath>

USING_NS_CC;

class Level : public Ref {
    UG_PROPERTY_GETTER(int, level, Level);
    UG_PROPERTY_GETTER(float, star, Star);
    UG_PROPERTY_GETTER(Man*, man, Man);
    UG_PROPERTY_GETTER(Vector<__Float*>*, stops, Stops);
    UG_PROPERTY_GETTER(Vector<SpawnPoint*>*, spawnPoints, SpawnPoints);
    UG_PROPERTY_GETTER(Vector<Wall*>*, walls, Walls);
    UG_PROPERTY_GETTER(Vector<Elevator*>*, elevators, Elevators);
    UG_PROPERTY_GETTER(Vector<Barrel*>*, barrels, Barrels);
    UG_PROPERTY_GETTER(Vector<Zombie*>*, zombies, Zombies);
    UG_PROPERTY_GETTER(Vector<TheShot*>*, shots, Shots);
    UG_PROPERTY_FULL(RectBody*, endZoneBody, EndZoneBody);
    
    UG_PROPERTY_GETTER(float, blockLeft, BlockLeft);
    UG_PROPERTY_GETTER(float, blockRight, BlockRight);
    UG_PROPERTY_GETTER(float, blockUp, BlockUp);
    UG_PROPERTY_GETTER(float, blockDown, BlockDown);
    UG_PROPERTY_GETTER(float, airX, AirX);
    UG_PROPERTY_GETTER(int, currentCheckPoint, CurrentCheckPoint);
    UG_PROPERTY_FULL_BOOL(reachCheckPoint, ReachCheckPoint);
    UG_PROPERTY_FULL(float, gameTime, GameTime);
    UG_PROPERTY_FULL(int, score, Score);
protected:
    float cameraTarget;
private:
    void callEnemies();
    void callEnemiesAtCheckPoint();
    
    void updateMan(float dt);
    void updateZombie(Zombie* zombie, float dt);
public:
    Level(int level);
    virtual ~Level();
    
    void update(float dt);
    void loadMap();
    
    void nextCheckPoint();
    void registerDamage(RectBody* damage, AnimatedObject* owner, int damageType);
    void createEndzone(Size contentSize);
};

#endif