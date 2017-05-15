//
//  ConvergenceEffect.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/05.
//
//

#include "ConvergenceEffect.h"
#include "CommonDefine.h"

ConvergenceEffectLayer::ConvergenceEffectLayer()
{
    centerPos = CCPointZero;
    widthR=200;
    lifeTime = 3.0;
}

ConvergenceEffectLayer::~ConvergenceEffectLayer()
{
    centerPos = CCPointZero;
    widthR=200;
    lifeTime = 3.0;
}

bool ConvergenceEffectLayer::init()
{
    if ( !CCLayer::init() ) {
        return false;
    }
    
    return true;
}

void ConvergenceEffectLayer::go()
{

    
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("img_circle_purple2.png");
    sprite->setPosition(centerPos);
    sprite->setScale(0.0);
    
    CCSprite* rotateSp = CCSprite::createWithSpriteFrameName("ring_circle.png");
    rotateSp->setPosition(ccp(sprite->getContentSize().width/2.0, sprite->getContentSize().height/2.0));
    sprite->addChild(rotateSp);
    rotateSp->runAction(CCRotateBy::create(20, 2160));
    rotateSp->setScale(0.5);

    
    CCEaseBackOut* scale1 = CCEaseBackOut::create(CCScaleTo::create(0.3, 0.3));
    CCEaseBackOut* scale2 = CCEaseBackOut::create(CCScaleTo::create(0.05, 0.2));
    CCEaseBackOut* scale21 = CCEaseBackOut::create(CCScaleTo::create(0.25, 0.6));
    CCEaseBackOut* scale3 = CCEaseBackOut::create(CCScaleTo::create(0.05, 0.5));
    CCEaseBackOut* scale31 = CCEaseBackOut::create(CCScaleTo::create(0.25, 1.0));
    CCEaseBackOut* scale4 = CCEaseBackOut::create(CCScaleTo::create(0.05, 0.9));
    CCEaseBackOut* scale5 = CCEaseBackOut::create(CCScaleTo::create(0.25, 1.3));
    CCEaseBackOut* scale7 = CCEaseBackOut::create(CCScaleTo::create(0.05, 1.2));
    CCEaseBackOut* scale8 = CCEaseBackOut::create(CCScaleTo::create(0.25, 1.3));
    CCEaseBackOut* scale9 = CCEaseBackOut::create(CCScaleTo::create(0.05, 1.25));
    CCEaseBackOut* scale10 = CCEaseBackOut::create(CCScaleTo::create(0.25, 1.3));
    CCEaseOut* scale6 = CCEaseOut::create(CCScaleTo::create(1.0, 0.0),1.0);
    sprite->runAction(CCSequence::create(CCDelayTime::create(1.5), scale1, CCDelayTime::create(1.5), scale2, scale21, CCDelayTime::create(1.5), scale3,scale31, CCDelayTime::create(1.5), scale4, scale5, CCDelayTime::create(1.5), scale7, scale8,CCDelayTime::create(1.5), scale9, scale10, CCDelayTime::create(2.0), scale6, NULL));

    CCActionInterval* act = CCScaleTo::create(8.5, 1.3);
//    sprite->runAction(CCSequence::create(act, CCDelayTime::create(2.5), scale6, NULL));
    
    addChild(sprite);

    createConvergenceSprite(sprite, 0.0);
    createConvergenceSprite(sprite, 0.5);
    createConvergenceSprite(sprite, 1.0);

    
    schedule(schedule_selector(ConvergenceEffectLayer::update), 0.04);
    CCCallFunc* stopSchedule = CCCallFunc::create(this, callfunc_selector(ConvergenceEffectLayer::stopSchedule));
    CCCallFunc* removeLayer = CCCallFunc::create(this, callfunc_selector(ConvergenceEffectLayer::removeFromParent));
    runAction(CCSequence::create(CCDelayTime::create(lifeTime-3.0), stopSchedule, CCDelayTime::create(8.0), removeLayer, NULL));
    
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_CHARACTORLAYER_EDIT_MAGICAL_CIRCLE);
}

void ConvergenceEffectLayer::createConvergenceSprite(cocos2d::CCSprite *sprite, float delay)
{
    CCSprite* convergenceSprite =CCSprite::createWithSpriteFrameName("img_circle_purple3.png");
    convergenceSprite->setOpacity(0); 
    CCEaseIn* csAction1 = CCEaseIn::create(CCScaleTo::create(1.5, 0.0),2.0);
    CCFadeIn* csAction4 = CCFadeIn::create(0.2);
    CCDelayTime* csAction7 = CCDelayTime::create(1.0);
    CCFadeOut* csAction6 = CCFadeOut::create(0.2);
    CCSequence* csAction8 = CCSequence::create(csAction4, csAction7, csAction6, NULL);
    CCSpawn* csAction5 = CCSpawn::create(csAction1, csAction8, NULL);
    CCActionInterval* csAction2 = CCScaleTo::create(0.0, 1.0);
    CCRepeat* csAction3 = CCRepeat::create(CCSequence::create(csAction5, csAction2, NULL),7);
    convergenceSprite->runAction(CCSequence::create(CCDelayTime::create(delay),csAction3,NULL));
    convergenceSprite->setPosition(sprite->getPosition());
//    convergenceSprite->setPosition(ccp(sprite->getContentSize().width/2.0, sprite->getContentSize().height/2.0));
    addChild(convergenceSprite);

}

void ConvergenceEffectLayer::stopSchedule()
{
    unschedule(schedule_selector(ConvergenceEffectLayer::update));
}

void ConvergenceEffectLayer::update(float delta)
{
    ConvergenceEffect* effect = ConvergenceEffect::create();
    effect->setCentetPos(centerPos);
    effect->go();
    addChild(effect);
}


ConvergenceEffect::ConvergenceEffect()
{
    centerPos = CCPointZero;
    widthR=250;
}

ConvergenceEffect::~ConvergenceEffect()
{
    centerPos = CCPointZero;
    widthR=250;
}

bool ConvergenceEffect::init()
{
    if ( !CCSprite::initWithSpriteFrameName("fly_star_pink.png") ) {
        return false;
    }
    return true;

}

ConvergenceEffect* ConvergenceEffect::create()
{
    ConvergenceEffect* pRet = new ConvergenceEffect();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

void ConvergenceEffect::go()
{
    int deg = arc4random() % 360;
 //   CCLOG("%d", deg);
    float r = arc4random() % 50 + widthR;
    float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg))+centerPos.x;
    float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg))+centerPos.y;
    this->setPosition(ccp(startPosX,startPosY));
    this->setScale(0.25);
    this->setOpacity(0);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CCSprite::removeFromParent));
    CCActionInterval* move = CCMoveTo::create(1.0, centerPos);
    CCActionInterval* scale = CCScaleTo::create(1.0, 0.1);
    CCEaseIn* move2 = CCEaseIn::create(move, 2.8);
    CCSequence* seq = CCSequence::create(CCFadeIn::create(0.1), CCDelayTime::create(0.8), CCFadeOut::create(0.1), NULL);
    CCSpawn* spawn = CCSpawn::create(seq, move2, scale, NULL);
    runAction(CCSequence::create(spawn, func, NULL));
    
}