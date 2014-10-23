#ifndef FACE_H
#define FACE_H

#include "cocos2d.h"
#include "FaceState.h"
#include "../UGCustomMacros.h"

USING_NS_CC;

class Face: public Sprite {
    UG_PROPERTY_FULL_BOOL(allowUpdate, AllowUpdate);
	UG_PROPERTY_FULL(char*, name, Name);
	UG_PROPERTY_FULL_REFF(cocos2d::Point, oOVector, OOVector);
    UG_PROPERTY_GETTER(__Dictionary*, states, States);
protected:
	Action* state;
private:
    virtual void onEnter();
    virtual void onExit();
public:
	Face();
	virtual ~Face();

	void registerState(const std::string& stateName, const std::string& patternName);
	void registerState(const std::string& stateName, Action* action);
    void removeState(const std::string& stateName);
	void setState(Action* state);
    void setState(const std::string& stateName);
    
    virtual bool init() override;
    virtual bool initWithFile(const std::string& filename) override;
    virtual bool initWithSpriteFrameName(const std::string& spriteFrameName) override;
    
	Action* createState(const std::string& patternName, float frameDuration, bool forever);
	Action* createState(const std::string& patternName, int frameNumber, float frameDuration, bool forever, bool reverse);
	Action* createState(const std::string& patternName, int startFrame, int endFrame, float frameDuration, bool forever, bool reverse);
	Action* getState();
	Action* getState(const std::string& stateName);
    
	Vector<SpriteFrame*>* createFrameVector(const std::string& patternName, int startFrame, int endFrame);
    void initFaceForKey(std::string key, __Array* faceStates);
};
#endif
