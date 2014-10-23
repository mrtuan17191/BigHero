#include "SoundModel.h"

static SoundModel* sharedSoundManager = NULL;

SoundModel* SoundModel::getInstance(void) {
	if (!sharedSoundManager) {
		sharedSoundManager = new SoundModel();
	}
	return sharedSoundManager;
}

SoundModel::SoundModel() {
    this->audioEngine = SimpleAudioEngine::getInstance();
    this->cmdFadeTime = 0.0f;
    this->currentFadeTime = 0.0f;
    this->currentBgMusicVolume = -1.0f;
    this->backgroundMusicId = -1;
    this->effectMap = UGMap<int, int>::create(50);
    this->soundCommandList = CCArray::createWithCapacity(50);
    CC_SAFE_RETAIN(this->soundCommandList);
}

SoundModel::~SoundModel(void) {
	delete effectMap;
    Utils::safeRelease(this->soundCommandList);
}

void SoundModel::update(float dt) {
    bool isFadeOut = false;
    for (unsigned int i = 0; i < soundCommandList->count(); i++) {
        SoundCommand* cmd = dynamic_cast<SoundCommand*>(soundCommandList->getObjectAtIndex(i));
        // Check type
        if (cmd == NULL) {
            continue;
        }
        
        // Update timeout
        float timeout = cmd->getTimeout();
        timeout = timeout - dt > 0 ? timeout - dt : 0.0f;
        cmd->setTimeout(timeout);
        
        // Check timeout is runout
        if (timeout > 0) {
            continue;
        }
        
        if (cmd->getFadeTime() > 0.0f) {
            this->currentFadeTime += dt;
            this->cmdFadeTime = cmd->getFadeTime();
            if (this->currentBgMusicVolume == -1.0f) {
                this->currentBgMusicVolume = this->getBackgroundMusicVolume();
            }
            float musicVolume = this->currentBgMusicVolume - (this->currentFadeTime / this->cmdFadeTime);
            if (musicVolume <= 0.0f) {
                this->currentFadeTime = 0.0f;
            } else {
                isFadeOut = true;
                this->setBackgroundMusicVolume(musicVolume);
                continue;
            } 
        }
        this->executeCmd(cmd);
        this->soundCommandList->removeObjectAtIndex(i);
    }
    
    if (this->currentBgMusicVolume != -1.0f && isFadeOut == false) {
        this->currentFadeTime += dt;
        float musicVolume = this->getBackgroundMusicVolume() + (this->currentFadeTime / this->cmdFadeTime);
        if (musicVolume >= this->currentBgMusicVolume) {
            this->setBackgroundMusicVolume(this->currentBgMusicVolume);
            this->currentBgMusicVolume = -1.0f;
        } else {
            this->setBackgroundMusicVolume(musicVolume);
        }
    }
}

void SoundModel::executeCmd(SoundCommand* cmd) {
    if (cmd->getCmdType() == StartCmd) {
        if (cmd->getSoundType() == BackGroundMusic) {
            if (this->backgroundMusicId == cmd->getSoundId()) {
                return;
            }
            char* path = getSoundFilePath(cmd->getSoundId());            
			this->audioEngine->playBackgroundMusic(std::string(CCFileUtils::getInstance()->fullPathForFilename(path)).c_str(), cmd->isLoop());
            this->backgroundMusicId = cmd->getSoundId();
        } else if (cmd->getSoundType() == EffectSound) {
            if (this->effectMap->containsKey(cmd->getSoundId())) {
                return;
            } 
            char* path = this->getSoundFilePath(cmd->getSoundId());
            int effectId = this->audioEngine->playEffect(std::string(CCFileUtils::getInstance()->fullPathForFilename(path)).c_str(), cmd->isLoop());
            if (cmd->isLoop()) {
                this->effectMap->add(cmd->getSoundId(), effectId);
            }
        }
    } else if (cmd->getCmdType() == StopCmd) {
        if (cmd->getSoundType() == BackGroundMusic) {
            this->audioEngine->stopBackgroundMusic(true);
            this->backgroundMusicId = -1;
        } else if (cmd->getSoundType() == EffectSound) {
            if (!this->effectMap->containsKey(cmd->getSoundId())) {
                return;
            }
            int effectId = this->effectMap->get(cmd->getSoundId());
            this->audioEngine->stopEffect(effectId);
            this->effectMap->remove(cmd->getSoundId());
        } else if(cmd->getSoundType() == AllEffects) {
            this->audioEngine->stopAllEffects();
            delete effectMap;
            this->effectMap = UGMap<int, int>::create(50);
        } else if(cmd->getSoundType() == AllSounds) {
            this->audioEngine->stopBackgroundMusic();
            this->audioEngine->stopAllEffects();
            this->backgroundMusicId = -1;
            delete effectMap;
            this->effectMap = UGMap<int, int>::create(50);
        }
    } else if (cmd->getCmdType() == PauseCmd) {
        if (cmd->getSoundType() == BackGroundMusic) {
            this->audioEngine->pauseBackgroundMusic();
        } else if (cmd->getSoundType() == EffectSound) {
            if (!this->effectMap->containsKey(cmd->getSoundId())) {
                return;
            }
            int effectId = this->effectMap->get(cmd->getSoundId());
            this->audioEngine->pauseEffect(effectId);
        } else if (cmd->getSoundType() == AllEffects) {
            this->audioEngine->pauseAllEffects();
        } else if (cmd->getSoundType() == AllSounds) {
            this->audioEngine->pauseBackgroundMusic();
            this->audioEngine->pauseAllEffects();
        }
    } else if (cmd->getCmdType() == ResumeCmd) {
        if (cmd->getSoundType() == BackGroundMusic) {
            this->audioEngine->resumeBackgroundMusic();
        } else if (cmd->getSoundType() == EffectSound) {
            if (!this->effectMap->containsKey(cmd->getSoundId())) {
                return;
            }
            int effectId = this->effectMap->get(cmd->getSoundId());
            this->audioEngine->resumeEffect(effectId);
        } else if (cmd->getSoundType() == AllEffects) {
            this->audioEngine->resumeAllEffects();
        } else if (cmd->getSoundType() == AllSounds) {
            this->audioEngine->resumeBackgroundMusic();
            this->audioEngine->resumeAllEffects();
        }
    }
}

char* SoundModel::getSoundFilePath(int soundId) {
    char* path = new char[128];
    char* extension = (char*) "mp3";
    if (soundId >= 99) {
        extension = (char*) "mp3";
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sprintf(path, "sound-%d.%s", soundId, extension);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    sprintf(path, "sound-%d.%s", soundId, extension);
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sprintf(path, "sounds/sound-%d.%s", soundId, extension);
#endif
    
    return path;
}

void SoundModel::addCmd(int soundId, int soundType, int cmdType, bool isLoop, float timeout, float fadeTime) {
    SoundCommand* cmd = SoundCommand::create();
    cmd->setSoundId(soundId);
    cmd->setLoop(isLoop);
    cmd->setTimeout(timeout);
    cmd->setCmdType(cmdType);
    cmd->setFadeTime(fadeTime);
    cmd->setSoundType(soundType);
    this->soundCommandList->addObject(cmd);
}

void SoundModel::addCmd(int soundId, int soundType, int cmdType, bool isLoop, float timeout) {
    this->addCmd(soundId, soundType, cmdType, isLoop, timeout, 0.0f);
}

void SoundModel::playBackgroundMusic(int soundId, bool isLoop, float timeout, float fadeTime) {
    this->addCmd(soundId, BackGroundMusic, StartCmd, isLoop, timeout, fadeTime);
}

void SoundModel::playEffect(int soundId, bool isLoop, float timeout) {
    this->addCmd(soundId, EffectSound, StartCmd, isLoop, timeout);
}

void SoundModel::stopBackgroundMusic(float timeout, bool isFadeOut) {
    if (isFadeOut) {
        this->addCmd(-1, BackGroundMusic, StopCmd, false, 0, timeout);
    } else {
        this->addCmd(-1, BackGroundMusic, StopCmd, false, timeout);
    }
}

void SoundModel::stopEffect(int soundId, float timeout) {
    this->addCmd(soundId, EffectSound, StopCmd, false, timeout);
}

void SoundModel::stopAllEffects(float timeout) {
    this->addCmd(-1, AllEffects, StopCmd, false, timeout);
}

void SoundModel::stopAllSounds(float timeout) {
    this->addCmd(-1, AllSounds, StopCmd, false, timeout);
}

float SoundModel::getEffectsVolume() {
	return this->audioEngine->getEffectsVolume();
}

float SoundModel::getBackgroundMusicVolume() {
	return this->audioEngine->getBackgroundMusicVolume();
}

void SoundModel::setEffectsVolume(float value) {
	this->audioEngine->setEffectsVolume(value);
}

void SoundModel::setBackgroundMusicVolume(float value) {
	this->audioEngine->setBackgroundMusicVolume(value);
}

void SoundModel::pauseBackgroundMusic(float timeout) {
	this->addCmd(-1, BackGroundMusic, PauseCmd, false, timeout);
}

void SoundModel::pauseEffect(int soundId, float timeout) {
    this->addCmd(soundId, EffectSound, PauseCmd, false, timeout);
}

void SoundModel::pauseAllEffects(float timeout) {
    this->addCmd(-1, AllEffects, PauseCmd, false, timeout);
}

void SoundModel::pauseAllSounds(float timeout) {
    this->addCmd(-1, AllSounds, PauseCmd, false, timeout);
}

void SoundModel::resumeEffect(int soundId, float timeout) {
	this->addCmd(soundId, EffectSound, ResumeCmd, false, timeout);
}

void SoundModel::resumeBackgroundMusic(float timeout) {
    this->addCmd(-1, BackGroundMusic, ResumeCmd, false, timeout);
}

void SoundModel::resumeAllEffects(float timeout) {
    this->addCmd(-1, AllEffects, ResumeCmd, false, timeout);
}

void SoundModel::resumeAllSounds(float timeout) {
    this->addCmd(-1, AllSounds, ResumeCmd, false, timeout);
}

