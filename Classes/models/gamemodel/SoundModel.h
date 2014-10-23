#ifndef SOUND_MODEL_H
#define SOUND_MODEL_H

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SoundCommand.h"
#include "../../utils/include/utils.h"

USING_NS_CC;
using namespace CocosDenshion;

class SoundModel : public Ref {
public:
	static SoundModel* getInstance(void);
    virtual void update(float dt);
    
	void playBackgroundMusic(int soundId, bool isLoop, float timeout, float fadeTime);
	void playEffect(int soundId, bool isLoop, float timeout);
	void stopBackgroundMusic(float timeout, bool isFadeOut);
	void stopEffect(int soundId, float timeout);
    void stopAllEffects(float timeout);
	void stopAllSounds(float timeout);
    void setEffectsVolume(float value);
	void setBackgroundMusicVolume(float value);
	void pauseBackgroundMusic(float timeout);
    void pauseEffect(int soundId, float timeout);
    void pauseAllEffects(float timeout);
    void pauseAllSounds(float timeout);
	void resumeBackgroundMusic(float timeout);
	void resumeEffect(int soundId, float timeout);
	void resumeAllEffects(float timeout);
    void resumeAllSounds(float timeout);
    
	float getEffectsVolume();
	float getBackgroundMusicVolume();
protected:
    SoundModel();
    virtual ~SoundModel();
    
    SimpleAudioEngine* audioEngine;
	int backgroundMusicId; // This holds current music id
    float currentFadeTime; // This holds current fade out time
    float cmdFadeTime; // This holds command fade out time
    float currentBgMusicVolume; // This holds current background music volume
    UGMap<int,int>* effectMap; // This holds set of effects will execute in next update
    __Array* soundCommandList; // This holds entire sound commands
    
    void addCmd(int soundId, int soundType, int cmdType, bool isLoop, float timeout, float fadeTime);
    void addCmd(int soundId, int soundType, int cmdType, bool isLoop, float timeout);
    void executeCmd(SoundCommand* cmd);
public:
    char* getSoundFilePath(int soundId);
};

#endif
