#include "SoundCommand.h"

SoundCommand::SoundCommand() {
}

SoundCommand::~SoundCommand() {
}

bool SoundCommand::init() {
    this->soundId = -1;
    this->soundType = 0;
    this->cmdType = 0;
    this->timeout = 0.0f;
    this->fadeTime = 0.0f;
    this->loop = false;
	return true;
}