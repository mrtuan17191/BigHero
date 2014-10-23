#include "Face.h"

Face::Face() {
	this->state = NULL;
	this->name = NULL;
    this->allowUpdate = true;
    this->autorelease();

    this->states = __Dictionary::create();
	CC_SAFE_RETAIN(this->states);
}

Face::~Face() {
}

void Face::onEnter() {
    CCSprite::onEnter();
}

void Face::onExit() {
    CCSprite::onExit();
    CC_SAFE_RELEASE(this->states);
    this->removeAllChildrenWithCleanup(true);
}

bool Face::init() {
    return Sprite::init();
}

bool Face::initWithFile(const std::string& filename) {
    return Sprite::initWithFile(filename);
}

bool Face::initWithSpriteFrameName(const std::string& spriteFrameName) {
    return Sprite::initWithSpriteFrameName(spriteFrameName);
}

Action* Face::getState() {
	return this->state;
}

Action* Face::getState(const std::string& stateName) {
	return (Action*) this->states->objectForKey(stateName);
}

void Face::setState(Action* s) {
	if (this->state != s) {
		if (this->state != NULL) {
			this->stopAction(state);
		}
		if (s != NULL) {
			this->state = s;
			this->runAction(this->state);
		} else {
			this->state = NULL;
		}
	}
}

void Face::setState(const std::string& stateName) {
    this->setState(this->getState(stateName));
}

void Face::registerState(const std::string& stateName, const std::string& patternName) {
	this->registerState(stateName, createState(patternName, 0.8f, true));
}

void Face::registerState(const std::string& stateName, Action* action) {
	if (action != NULL) {
		this->states->setObject(action, stateName);
	}
}

void Face::removeState(const std::string &stateName) {
    this->states->removeObjectForKey(stateName);
}

Action* Face::createState(const std::string&  patternName, float frameDuration, bool forever) {
	return this->createState(patternName, 4, frameDuration, forever, false);
}

Action* Face::createState(const std::string&  patternName, int frameNumber, float frameDuration, bool forever, bool reverse) {
	return this->createState(patternName, 1, frameNumber, frameDuration, forever, reverse);
}

Action* Face::createState(const std::string& patternName, int startFrame, int endFrame, float frameDuration, bool forever, bool reverse) {
    Vector<SpriteFrame*> *framesVector = this->createFrameVector(patternName,startFrame,endFrame);
	if (reverse) {
		int framesSize = framesVector->size();
		for(int i = framesSize - 1; i >= 0; i--) {
			framesVector->pushBack(framesVector->at(i));
		}
	}
    
    float delay = frameDuration / framesVector->size();
    Animation* animation = Animation::createWithSpriteFrames(*framesVector, delay);
	Animate* animate = Animate::create(animation);
	Action* action = NULL;
	if (forever) {
		action = RepeatForever::create(animate);
	} else {
		action = (Action*) animate;
	}
	return action;
}

Vector<SpriteFrame*>* Face::createFrameVector(const std::string& patternName, int startFrame, int endFrame) {
	SpriteFrameCache *spriteFrameCache = SpriteFrameCache::getInstance();
    Vector<SpriteFrame*>*framesVector = new Vector<SpriteFrame*>;
	char frameName[128];
	for(int i = startFrame; i <= endFrame; i++){
		sprintf(frameName, patternName.c_str(), i);
		framesVector->pushBack(spriteFrameCache->getSpriteFrameByName(frameName));
	}
	return framesVector;
}

void Face::initFaceForKey(std::string key, __Array* faceStates) {
    // load file plist
    auto frameCache = SpriteFrameCache::getInstance();
    char plistFile[200];
    sprintf(plistFile, "%s.plist", key.data());
    frameCache->addSpriteFramesWithFile(plistFile);
    
    // load animation
    auto animationCache = AnimationCache::getInstance();
    char animationFile[200];
    std::string firstSpriteFrameName = "";
    sprintf(animationFile, "ani-%s.plist", key.data());
    std::string path = FileUtils::getInstance()->fullPathForFilename(animationFile);
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(path);
    ValueVector animations = dict.at("Animations").asValueVector();
    __Array* animationNames = __Array::create();
    
    for (int i = 0; i < animations.size(); i++) {
        ValueMap animationDict = animations.at(i).asValueMap();
        ValueVector frames = animationDict.at("Frames").asValueVector();
        ValueMap properties = animationDict.at("Properties").asValueMap();
        std::string name = properties.at("Animation").asString();
        animationNames->addObject(__String::create(name.data()));
        float delayPerUnit = 1 / properties.at("FPS").asFloat();
        Animation* animation = NULL;
        
        if (frames.empty()) {
            continue;
        }
        
        ssize_t frameSize = frames.size();
        Vector<AnimationFrame*> animationFrames(frameSize);
        
        for (auto& frame : frames) {
            ValueMap frameValueMap = frame.asValueMap();
            std::string frameString = frameValueMap.at("FrameName").asString();
            if (firstSpriteFrameName == "") {
                firstSpriteFrameName = frameString;
            }
            float duration = frameValueMap.at("Duration").asFloat();
            SpriteFrame* spriteFrame = frameCache->getSpriteFrameByName(frameString);
            
            if (!spriteFrame) {
                continue;
            }
            
            AnimationFrame* animFrame = AnimationFrame::create(spriteFrame, duration, ValueMap());
            animationFrames.pushBack(animFrame);
        }
        
        if (frames.empty()) {
            assert(NULL);
            continue;
        } else if (frames.size() != frameSize) {
            assert(NULL);
        }
        
        animation = Animation::create(animationFrames, delayPerUnit);
        animationCache->addAnimation(animation, name);
    }
    
    this->initWithSpriteFrameName(firstSpriteFrameName);
    this->getTexture()->setAliasTexParameters();
    
    for (int i = 0; i < animationNames->count(); i++) {
        __String* animationName = (__String*) animationNames->getObjectAtIndex(i);
        std::string animationNameStr = animationName->getCString();
        for (int j = 0; j < faceStates->count(); j++) {
            FaceState* state = (FaceState*) faceStates->getObjectAtIndex(j);
            std::string stateString = state->getStateName();
            if (animationNameStr.find(stateString) != std::string::npos) {
                auto animation = animationCache->getAnimation(animationNameStr);
                auto animate = Animate::create(animation);
                if (state->isLoop()) {
                    Action* action = RepeatForever::create(animate);
                    this->registerState(stateString, action);
                } else {
                    Action* action = (Action*) animate;
                    this->registerState(stateString, action);
                }
                break;
            }
        }
    }
}