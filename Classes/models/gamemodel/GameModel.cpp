#include "GameModel.h"

static GameModel* gameModelInstance = NULL;

GameModel::GameModel() {
    this->currentLevel = NULL;
    this->savedData = NULL;
    this->mapData = NULL;
    this->gameState = GS_WAIT;
    this->level = -1;
    this->currentDelayToNextMap = -1;
    this->currentDelayToTransit = -1;
    this->animations = __Array::create();
    CC_SAFE_RETAIN(this->animations);
    
    Director::getInstance()->getScheduler()->scheduleUpdate(this, SCHEDULE_GAME_MODEL, false);
    this->loadData();
    this->loadAnimations();
}

GameModel::~GameModel() {
    Utils::safeRelease(this->currentLevel);
    Utils::safeRelease(this->savedData);
    Utils::safeRelease(this->animations);
}

GameModel* GameModel::getInstance() {
    if (gameModelInstance == NULL) {
        gameModelInstance = new GameModel();
        CC_SAFE_RETAIN(gameModelInstance);
    }
    return gameModelInstance;
}

void GameModel::loadData() {
    this->mapData = new MapData();
    CC_SAFE_RETAIN(this->mapData);
    {
        Json* root;
        string fileData = Utils::readFile(DOC_MAPS, false, true, true);
        root = Json_create(fileData.data());
        
        if (root) {
            Json* maps = Json_getItem(root, "maps");
            int mapSize = maps->size;
            for (int i = 0; i < mapSize; i++) {
                Json* map = Json_getItemAt(maps, i);
                string name = Json_getString(map, "name", "");
                int requireSocial = Json_getInt(map, "requireSocial", 0);
                
                __Dictionary* data = __Dictionary::create();
                data->setObject(__String::create(name), "name");
                data->setObject(__Bool::create(requireSocial == 1 ? true : false), "requireSocial");
                this->mapData->getLevels()->pushBack(data);
            }
        }
    }
    
    this->savedData = new SavedData();
    CC_SAFE_RETAIN(this->savedData);
    {
        string fileData = Utils::readFile(DOC_USER, true, false, true);
        if (fileData == "") {
            fileData = Utils::readFile(DOC_USER, false, true, true);
        }
        if (fileData != "") {
            __Array* splits = Utils::split(fileData, '$');
            __String* level = (__String*) splits->getObjectAtIndex(0);
            __String* musicOn = (__String*) splits->getObjectAtIndex(1);
            __String* effectOn = (__String*) splits->getObjectAtIndex(2);
            __String* sharedSocial = (__String*) splits->getObjectAtIndex(3);
            
            int levelInt = atoi(level->getCString());
            bool musicOnBool = atoi(musicOn->getCString()) == 1 ? true : false;
            bool effectOnBool = atoi(effectOn->getCString()) == 1 ? true : false;
            bool sharedSocialBool = atoi(sharedSocial->getCString()) == 1 ? true : false;
            
            this->savedData->setLevel(levelInt);
            this->savedData->setMusicOn(musicOnBool);
            this->savedData->setSfxOn(effectOnBool);
            this->savedData->setSharedSocial(sharedSocialBool);
        }
    }
}

void GameModel::loadAnimations() {
    this->animations->addObject(new FaceState(STATE_IDLE, true));
    this->animations->addObject(new FaceState(STATE_RUN, true));
    this->animations->addObject(new FaceState(STATE_ATTACK, false));
    this->animations->addObject(new FaceState(STATE_JUMP, true));
    this->animations->addObject(new FaceState(STATE_DIE, true));
    this->animations->addObject(new FaceState(STATE_SHOOT, true));
    this->animations->addObject(new FaceState(STATE_HIT_HIGH, false));
    this->animations->addObject(new FaceState(STATE_HIT_LOW, false));
    this->animations->addObject(new FaceState(STATE_KO, false));
    this->animations->addObject(new FaceState(STATE_GET_UP, false));
    this->animations->addObject(new FaceState(STATE_DRIVE, true));
    this->animations->addObject(new FaceState(STATE_SPIN, true));
    this->animations->addObject(new FaceState(STATE_DASH, true));
    this->animations->addObject(new FaceState(STATE_BLOCK, false));
    this->animations->addObject(new FaceState(STATE_SPAWN_UP, true));
    this->animations->addObject(new FaceState(STATE_SPAWN_DOWN, true));
    this->animations->addObject(new FaceState(STATE_HEAD1, true));
    this->animations->addObject(new FaceState(STATE_HEAD2, true));
    this->animations->addObject(new FaceState(STATE_WALL1_IDLE, true));
    this->animations->addObject(new FaceState(STATE_WALL2_IDLE, true));
    this->animations->addObject(new FaceState(STATE_WALL3_IDLE, true));
    this->animations->addObject(new FaceState(STATE_DOOR1_IDLE, true));
    this->animations->addObject(new FaceState(STATE_DOOR1_OPEN, false));
    this->animations->addObject(new FaceState(STATE_DOOR1_CLOSE, false));
    this->animations->addObject(new FaceState(STATE_DOOR2_IDLE, true));
    this->animations->addObject(new FaceState(STATE_DOOR2_OPEN, false));
    this->animations->addObject(new FaceState(STATE_DOOR2_CLOSE, false));
    this->animations->addObject(new FaceState(STATE_DOOR3_IDLE, true));
    this->animations->addObject(new FaceState(STATE_DOOR3_OPEN, false));
    this->animations->addObject(new FaceState(STATE_DOOR3_CLOSE, false));
    this->animations->addObject(new FaceState(STATE_BLOOD, true));
    this->animations->addObject(new FaceState(STATE_BOMB_BEACON, true));
    this->animations->addObject(new FaceState(STATE_EXPLOSION, true));
    this->animations->addObject(new FaceState(STATE_EYE_FLASH, true));
    this->animations->addObject(new FaceState(STATE_GROUND_EXPLOSION, true));
    this->animations->addObject(new FaceState(STATE_HIT_FLASH, true));
    this->animations->addObject(new FaceState(STATE_HIT_FLASH_2, false));
    this->animations->addObject(new FaceState(STATE_SHADOW, true));
    this->animations->addObject(new FaceState(STATE_STARS, true));
    this->animations->addObject(new FaceState(STATE_WALL_EXPLOSION, true));
    this->animations->addObject(new FaceState(STATE_EYE_LASER, true));
    this->animations->addObject(new FaceState(STATE_GRENADE, true));
    this->animations->addObject(new FaceState(STATE_ATTACK2, true));
    this->animations->addObject(new FaceState(STATE_HIT, false));
    this->animations->addObject(new FaceState(STATE_PICKUP, true));
    this->animations->addObject(new FaceState(STATE_AERIAL_ATTACK, false));
    this->animations->addObject(new FaceState(STATE_ATTACK_1, false));
    this->animations->addObject(new FaceState(STATE_ATTACK_2, false));
    this->animations->addObject(new FaceState(STATE_ATTACK_3, false));
    this->animations->addObject(new FaceState(STATE_ATTACK_4, false));
    this->animations->addObject(new FaceState(STATE_ATTACK_5, false));
    this->animations->addObject(new FaceState(STATE_FRENZY_ATTACK_1, false));
    this->animations->addObject(new FaceState(STATE_FRENZY_ATTACK_2, false));
    this->animations->addObject(new FaceState(STATE_FRENZY_ATTACK_3, false));
    this->animations->addObject(new FaceState(STATE_FRENZY_ATTACK_4, false));
    this->animations->addObject(new FaceState(STATE_FRENZY_ATTACK_5, false));
    this->animations->addObject(new FaceState(STATE_ARMED_IDLE, true));
    this->animations->addObject(new FaceState(STATE_ARMED_RUN, true));
    this->animations->addObject(new FaceState(STATE_ARMED_ATTACK_1, true));
    this->animations->addObject(new FaceState(STATE_ARMED_ATTACK_2, true));
    this->animations->addObject(new FaceState(STATE_ENRAGE_IDLE, true));
    this->animations->addObject(new FaceState(STATE_ENRAGE, true));
    this->animations->addObject(new FaceState(STATE_BULLET, true));
    this->animations->addObject(new FaceState(STATE_FIRE_LEFT, true));
    this->animations->addObject(new FaceState(STATE_FIRE_RIGHT, true));
    this->animations->addObject(new FaceState(STATE_WATER_POINT, true));
    this->animations->addObject(new FaceState(STATE_WATER_SPRAY, true));
    this->animations->addObject(new FaceState(STATE_FRAGS, true));
    this->animations->addObject(new FaceState(STATE_SPAWN, true));
    
    for (int i = 0; i < this->animations->count(); i++) {
        for (int j = i + 1; j < this->animations->count(); j++) {
            FaceState* stringLeft = (FaceState*) this->animations->getObjectAtIndex(i);
            FaceState* stringRight = (FaceState*) this->animations->getObjectAtIndex(j);
            
            string stringLeftString = stringLeft->getStateName();
            string stringRightString = stringRight->getStateName();
            
            if (stringLeftString.length() < stringRightString.length()) {
                this->animations->exchangeObject(stringLeft, stringRight);
            }
        }
    }
}

void GameModel::next() {
    if (this->currentLevel != NULL) {
        Utils::safeRelease(this->currentLevel);
        this->currentLevel = NULL;
    }
    
    this->level = this->level + 1;
    this->play(this->level);
}

bool GameModel::isLastMap() {
    int maxLevel = this->mapData->getLevelSize();
    if (this->level >= maxLevel) {
        return true;
    }
    return false;
}

void GameModel::transit(float time) {
    this->currentDelayToTransit = time;
}

void GameModel::replay() {
    if (this->currentLevel != NULL) {
        Utils::safeRelease(this->currentLevel);
        this->currentLevel = NULL;
    }
    
    this->play(this->level);
}

void GameModel::quit() {
    if (this->currentLevel != NULL) {
        Utils::safeRelease(this->currentLevel);
        this->currentLevel = NULL;
    }
    this->gameState = GS_WAIT;
}

void GameModel::pause() {
    this->gameState = GS_PAUSE;
}

void GameModel::resume() {
    this->gameState = GS_PLAY;
}

void GameModel::play(int level) {
    this->level = level;
    this->gameState = GS_LOAD;
    this->currentLevel = new Level(level);
    CC_SAFE_RETAIN(this->currentLevel);
    
    this->currentLevel->loadMap();
    
    GameEvent* startGameEvent = new GameEvent();
    startGameEvent->setEventCode(EVT_MAP_CREATED);
    this->fireEvent(startGameEvent);
}

void GameModel::update(float dt) {
    SoundModel* soundModel = SoundModel::getInstance();
    if (soundModel != NULL) {
        soundModel->update(dt);
        if (this->savedData->isMusicOn()) {
            soundModel->setBackgroundMusicVolume(1);
        } else {
            soundModel->setBackgroundMusicVolume(0);
        }
        
        if (this->savedData->isSfxOn()) {
            soundModel->setEffectsVolume(1);
        } else {
            soundModel->setEffectsVolume(0);
        }
    }
    
    if (this->gameState == GS_LOAD) {
        this->currentDelayToTransit -= dt;
        if (this->currentDelayToTransit < 0) {
            this->gameState = GS_PLAY;
        }
    }
    
    if (this->gameState == GS_PLAY) {
        this->currentLevel->update(dt);
        
        if (this->currentLevel->getMan()->isDead()) {
            this->gameState = GS_REPLAY;
            this->currentDelayToNextMap = CONF_DELAY_TO_NEXT_MAP;
        }
        
        if (this->currentLevel->getMan()->isDisappear()) {
            this->gameState = GS_FINISH;
            this->currentDelayToNextMap = CONF_DELAY_TO_NEXT_MAP;
        }
    }
    
    if (this->gameState == GS_FINISH) {
        this->currentLevel->update(dt);
        this->currentDelayToNextMap -= dt;
        if (this->currentDelayToNextMap < 0) {
            GameEvent* startGameEvent = new GameEvent();
            startGameEvent->setEventCode(EVT_NEXT_MAP);
            this->fireEvent(startGameEvent);
        }
    }
    
    if (this->gameState == GS_REPLAY) {
        this->currentLevel->update(dt);
        this->currentDelayToNextMap -= dt;
        if (this->currentDelayToNextMap < 0) {
            GameEvent* startGameEvent = new GameEvent();
            startGameEvent->setEventCode(EVT_REPLAY_MAP);
            this->fireEvent(startGameEvent);
        }
    }
}

void GameModel::save() {
    char savedText[200];
    int level = this->savedData->getLevel();
    int musicOn = this->savedData->isMusicOn() ? 1 : 0;
    int effectOn = this->savedData->isSfxOn() ? 1 : 0;
    int sharedSocial = this->savedData->isSharedSocial() ? 1 : 0;
    sprintf(savedText, TXT_SAVE_TEXT, level, musicOn, effectOn, sharedSocial);
    Utils::writeFile(savedText, DOC_USER, true);
}

void GameModel::updateLevel() {
    if (this->level > this->savedData->getLevel()) {
        this->savedData->setLevel(this->level);
    }
}

bool GameModel::checkLevelLock(int level) {
    int currentLevel = this->savedData->getLevel();
    if (level <= currentLevel) {
        return false;
    }
    return true;
}

bool GameModel::checkLevelRequireSocial(int level) {
    bool requireSocial = this->mapData->getLevelRequireSocial(level);
    if (requireSocial && !this->savedData->isSharedSocial()) {
        return false;
    }
    return true;
}
