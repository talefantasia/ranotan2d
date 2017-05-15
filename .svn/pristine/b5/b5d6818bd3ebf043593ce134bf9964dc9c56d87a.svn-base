//
//  File.h
//  lineloop
//
//  Created by Takahashi Hiroyuki on 2013/11/11.
//
//

#ifndef __lineloop__AnimationFactory__
#define __lineloop__AnimationFactory__

#include <cocos2d.h>

USING_NS_CC;

#include "../jsoncpp/json/json.h"

enum Reference
{
    TO,
    BY,
};

class AnimationFactory : public CCObject
{
public:
    AnimationFactory();
    virtual ~AnimationFactory();
    virtual bool init();
    CREATE_FUNC(AnimationFactory);
    
    CCActionInterval* getActions(const char* key);
    CCActionInterval* getActions(const char* key, CCCallFunc* withEndCallbackFunc);
    CCActionInterval* getActions(const char* key, CCDictionary* dic);
    CCActionInterval* getActions(const char* key, CCDictionary* dic, CCCallFunc* withEndCallbackFunc);
    
    static AnimationFactory* sharedInstance();

protected:
    Json::Value m_root;

    
private:
    CCActionInterval* doCreate(Json::Value& m_json, CCDictionary* dic);
    
    CCActionInterval* createSimpleAction(Json::Value& m_json, CCDictionary* dic);
    CCActionInterval* createSequence(Json::Value& m_json, CCDictionary* dic);
    CCActionInterval* createSpawn(Json::Value& m_json, CCDictionary* dic);
    CCActionInterval* createRepeat(Json::Value& m_json, CCDictionary* dic);
    
    CCActionInterval* createMove(Json::Value& m_json, CCDictionary* dic);
    CCActionInterval* createScale(Json::Value& m_json);
    CCActionInterval* createRotate(Json::Value& m_json);
    CCActionInterval* createJump(Json::Value& m_json);
    CCActionInterval* createBlink(Json::Value& m_json);
    CCActionInterval* createFadeout(Json::Value& m_json);
    CCActionInterval* createFadein(Json::Value& m_json);
    CCActionInterval* createFadeto(Json::Value& m_json);
    CCActionInterval* createWait(Json::Value& m_json, CCDictionary* dic);
    
    Reference changeReference(const char* reference);
    
    CCActionInterval* maskEasing(const char* easing, float fRate, CCActionInterval* action);
    
};

#endif /* defined(__lineloop__AnimationFactory__) */
