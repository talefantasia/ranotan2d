//
//  CommonButton.h
//  LineBubble
//
//  Created by nhn on 12. 9. 13..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#ifndef __COMMON_BUTTON_H__
#define __COMMON_BUTTON_H__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class CommonButton : public CCControlButton
{
public:
    CommonButton(void);
    virtual ~CommonButton(void);
    
    static CommonButton* getRunningButton();
    
    static CommonButton* create(CCScale9Sprite* sprite, const cocos2d::CCSize& btnSize, cocos2d::CCObject* target, cocos2d::SEL_CallFuncO selector);
    
    static CommonButton* createWithSpriteFrameName(const char *pszSpriteFrameName, cocos2d::CCObject* target, cocos2d::SEL_CallFuncO selector);
    
    static CommonButton* createWithSpriteFrameName(const char *pszSpriteFrameName, const cocos2d::CCRect& capInsets, const cocos2d::CCSize& btnSize, cocos2d::CCObject* target, cocos2d::SEL_CallFuncO selector);
    
    virtual void setHighlighted(bool enabled);
    
    void setTarget(cocos2d::CCObject *target, cocos2d::SEL_CallFuncO selector);
    
    void touchUpOutsideAction( cocos2d::CCObject* sender, CCControlEvent controlEvent);
    void touchUpInsideAction( cocos2d::CCObject* sender, CCControlEvent controlEvent);
    void touchDownAction( cocos2d::CCObject* sender, CCControlEvent controlEvent);
    
    void setButtonSoundName(const char* btnSound);
	
//	CC_SYNTHESIZE(cocos2d::CCObject*, m_pTarget, PTarget);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCString *, m_sBtnSound, BtnSound);
	CC_SYNTHESIZE_RETAIN(cocos2d::CCDictionary *, m_sParams, Params);
protected:
	cocos2d::SEL_CallFuncO    m_pfnCallFunc;
    cocos2d::CCObject* m_pTarget;
    
    
    void callbackAction(cocos2d::CCObject* obj);
};

#endif // __COMMON_BUTTON_H__
