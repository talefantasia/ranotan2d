//
//  StarFlyEffect.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/12.
//
//

#include "StarFlyEffect.h"

StarFlyEffect::StarFlyEffect()
{
    
}

StarFlyEffect::~StarFlyEffect()
{
    
}

bool StarFlyEffect::init()
{
    if ( !CCSprite::initWithSpriteFrameName("fly_star.png") ) {
        return false;
    }
    return true;
}

StarFlyEffect* StarFlyEffect::create()
{
    StarFlyEffect* pRet = new StarFlyEffect();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void StarFlyEffect::initActionRundom(int seed)
{
    int deg = seed % 30;
    float speed = (seed % 100)/100.0 +1.0;
    int width = seed % 50 + 100;
    float scale = (float)(seed % 10) / (100.0);
    initAction(deg, speed, width, scale);
}

void StarFlyEffect::initAction(int deg, float speed, float width, float scale)
{
    float posX = width * cos(CC_DEGREES_TO_RADIANS(deg))+getPosition().x;
    float posY = width * sin(CC_DEGREES_TO_RADIANS(deg))+getPosition().y;
    
    CCActionInterval* moveAction = CCMoveTo::create(2.2, ccp(posX,posY));
    CCActionInterval* scaleAction1 = CCScaleTo::create(0.2, 0.2);
    CCActionInterval* scaleAction2 = CCScaleTo::create(2.0, 0.0);
    CCSequence* seq = CCSequence::create(scaleAction1, scaleAction2, NULL);
   CCActionInterval* actions = CCSpawn::create(moveAction, seq, NULL);
    
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(StarFlyEffect::removeMySelf));
    
    this->runAction(CCSequence::create(actions,func, NULL));
}

void StarFlyEffect::removeMySelf()
{
    removeFromParentAndCleanup(true);
}