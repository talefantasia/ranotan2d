//
//  CCAsyncSprite.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/11.
//
//

#include "CCAsyncSprite.h"

CCAsyncSprite::CCAsyncSprite()
{
    
}

CCAsyncSprite::~CCAsyncSprite()
{
    
}

CCAsyncSprite* CCAsyncSprite::create(const char *filename)
{
    CCAsyncSprite* pRet = new CCAsyncSprite();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

CCAsyncSprite* CCAsyncSprite::createWithTexture(cocos2d::CCTexture2D *texture)
{
    CCAsyncSprite* pRet = new CCAsyncSprite();
    if (pRet && pRet->init(texture)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}



bool CCAsyncSprite::init(const char *filename)
{
    if ( !CCSprite::init() ) {
        return false;
    }
    
    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    CCLOG("%s", "texture load start");
    cache->addImageAsync(filename, this, callfuncO_selector(CCAsyncSprite::loadedImage));
    return true;
}


bool CCAsyncSprite::init(CCTexture2D* texture)
{
    if ( !CCSprite::initWithTexture(texture) ) {
        return false;
    }
    
    return true;
}

void CCAsyncSprite::loadedImage(cocos2d::CCObject *object)
{
    CCLOG("%s", "texture loaded");
    CCTexture2D* texture = (CCTexture2D*)object;
    setTexture(texture);
}