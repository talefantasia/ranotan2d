//
//  CommonButton.cpp
//  LineBubble
//
//  Created by nhn on 12. 9. 13..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include "CommonButton.h"
#include "ResolutionUtil.h"

USING_NS_CC;
//using namespace CocosDenshion;

enum {
    kCommonButtonActionTag = 1,    
};

static CommonButton* g_oRunningButton = NULL;

CommonButton::CommonButton(void)
{
    m_pTarget = NULL;
    m_pfnCallFunc = NULL;
	m_sBtnSound = NULL;
    m_sParams = NULL;
}

CommonButton::~CommonButton(void)
{
    if( g_oRunningButton == this ) {
        g_oRunningButton = NULL;
    }
	CC_SAFE_RELEASE_NULL(m_sBtnSound);
	CC_SAFE_RELEASE_NULL(m_sParams);
}

CommonButton*  CommonButton::getRunningButton()
{
    return g_oRunningButton;
}

CommonButton* CommonButton::create(CCScale9Sprite* sprite, const CCSize& btnSize, CCObject* target, SEL_CallFuncO selector)
{
    CommonButton *pRet = new CommonButton();
	pRet->setBtnSound(ccs("bubble_btn"));
    pRet->initWithBackgroundSprite(sprite);
    pRet->autorelease();
    pRet->setAdjustBackgroundImage(false);
    if( !btnSize.equals(CCSizeZero) ){
        pRet->setPreferredSize(btnSize);
    }
    pRet->setZoomOnTouchDown(false);
    pRet->setTarget(target, selector);
    pRet->addTargetWithActionForControlEvent(pRet, cccontrol_selector(CommonButton::touchDownAction), CCControlEventTouchDown);
    pRet->addTargetWithActionForControlEvent(pRet, cccontrol_selector(CommonButton::touchUpInsideAction), CCControlEventTouchUpInside);
    
    pRet->addTargetWithActionForControlEvent(pRet, cccontrol_selector(CommonButton::touchUpOutsideAction), CCControlEventTouchUpOutside);
	
    return pRet;
}

CommonButton* CommonButton::createWithSpriteFrameName(const char *pszSpriteFrameName, CCObject* target, SEL_CallFuncO selector)
{
    return createWithSpriteFrameName(pszSpriteFrameName, CCRectZero, CCSizeZero, target, selector);
}

CommonButton* CommonButton::createWithSpriteFrameName(const char *pszSpriteFrameName, const CCRect& capInsets, const CCSize& btnSize, CCObject* target, SEL_CallFuncO selector)
{
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
    
    CCRect rect = capInsets;
    
    if( !capInsets.equals(CCRectZero) ) {
        rect.origin.x = capInsets.origin.x + frame->getRect().origin.x;
        rect.origin.y = capInsets.origin.y + frame->getRect().origin.y;
        rect.size.width = capInsets.size.width;
        rect.size.height = capInsets.size.height;
    } else {
        rect = frame->getRect();
    }
    
    CCScale9Sprite* btnSprite = CCScale9Sprite::createWithSpriteFrame(frame, rect);
    
    if( !btnSize.equals(CCSizeZero) ){
        btnSprite->setContentSize(btnSize);
    }
    
    return create(btnSprite, btnSize, target, selector);
}

void CommonButton::setHighlighted(bool enabled)
{
    if( g_oRunningButton == NULL || g_oRunningButton == this )
    {
        g_oRunningButton = this;
        
        CCControl::setHighlighted(enabled);

        CCAction *action = getActionByTag(kCommonButtonActionTag);
        if (action)
        {
            stopAction(action);
//            g_oRunningButton = NULL;
        }

        CCAction* zoomAction;
        (isHighlighted() && isEnabled() && !isSelected()) ? zoomAction = CCScaleTo::create(0.01f, 1.1f * ResolutionUtil::getScaleRate()) : zoomAction = CCSequence::create(CCEaseBounceOut::create(CCScaleTo::create(0.2f, 1.0f * ResolutionUtil::getScaleRate())), CCCallFuncN::create(this, callfuncN_selector(CommonButton::callbackAction)), NULL);
        zoomAction->setTag(kCommonButtonActionTag);
        runAction(zoomAction);
    }
}

void CommonButton::setTarget(CCObject *target, SEL_CallFuncO selector)
{
	m_pTarget = target;
    m_pfnCallFunc = selector;
}

void CommonButton::callbackAction(CCObject* obj)
{
    if (isSelected() && m_pTarget && m_pfnCallFunc && isVisible())
    {
        (m_pTarget->*m_pfnCallFunc)(this);
    }
    setSelected(false);
    g_oRunningButton = NULL;
}

void CommonButton::touchDownAction( CCObject* sender, CCControlEvent controlEvent)
{
    if( g_oRunningButton == NULL || g_oRunningButton == this )
    {
        setSelected(false);
    }
}

void CommonButton::touchUpInsideAction( CCObject* sender, CCControlEvent controlEvent)
{
    if( g_oRunningButton == NULL || g_oRunningButton == this )
    {
        setSelected(true);
        
        //SimpleAudioEngine::sharedEngine()->playEffect( SoundUtils::sharedUtils()->getSoundFilePath(m_sBtnSound->getCString()) );
    }
}

void CommonButton::touchUpOutsideAction( CCObject* sender, CCControlEvent controlEvent)
{
    g_oRunningButton = NULL;
}

void CommonButton::setButtonSoundName(const char *btnSound)
{
	setBtnSound(ccs(btnSound));
}


