#ifndef SOUND_COMMAND_H
#define SOUND_COMMAND_H

#include "cocos2d.h"
#include "../../utils/include/utils.h"

USING_NS_CC;

enum SoundRepository {
};

enum SoundCommandType {
    StartCmd = 1,
    StopCmd = 2,
    PauseCmd = 3,
    ResumeCmd = 4
};

enum SoundType {
    BackGroundMusic = 1,
    EffectSound = 2,
    AllEffects = 3,
    AllSounds = 4,
};

class SoundCommand: public Ref {
    UG_PROPERTY_FULL(int, soundId, SoundId);
    UG_PROPERTY_FULL_BOOL(loop, Loop);
    UG_PROPERTY_FULL(float, fadeTime, FadeTime);
    UG_PROPERTY_FULL(int, soundType, SoundType);
	UG_PROPERTY_FULL(int, cmdType, CmdType);
    UG_PROPERTY_FULL(float, timeout, Timeout);
public:
	SoundCommand();
	virtual ~SoundCommand();
    virtual bool init();
    CREATE_FUNC(SoundCommand);
};
#endif
