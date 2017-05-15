//
//  OpacitySprite.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/08/20.
//
//

#include "OpacitySprite.h"

OpacitySprite::OpacitySprite()
{
    
}

OpacitySprite::~OpacitySprite()
{
    
}

bool OpacitySprite::init(const char *pszSpriteFrameName)
{
    if ( !CCSprite::initWithSpriteFrameName(pszSpriteFrameName) ) {
        return false;
    }
    return true;
}

OpacitySprite* OpacitySprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    OpacitySprite *pRet = new OpacitySprite();
    if (pRet && pRet->init(pszSpriteFrameName))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }

}

void OpacitySprite::setOpacity(GLubyte opacity)
{
    this->cocos2d::CCSprite::setOpacity(opacity);
    CCObject* child;
    CCARRAY_FOREACH(m_pChildren, child)
    {
        CCNode* pNode = (CCNode*)child;
        cocos2d::CCRGBAProtocol *pRGBAProtocol = dynamic_cast<cocos2d::CCRGBAProtocol*>(pNode);
        if (pRGBAProtocol)
        {
            pRGBAProtocol->setOpacity(opacity);
        }
    }
}