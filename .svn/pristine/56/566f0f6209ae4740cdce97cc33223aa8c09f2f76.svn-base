//
//  SubCharactor.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/04.
//
//

#include "SubCharactor.h"
#include "CommonDefine.h"

SubCharactor::SubCharactor()
{
    status = SubCharactorStatus::Status_Normal;
    basePos = CCPointZero;
}

SubCharactor::~SubCharactor()
{
    status = SubCharactorStatus::Status_Normal;
    basePos = CCPointZero;
    deleteNotification();
}

void SubCharactor::deleteNotification()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_ROLLING);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP_CHECK);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMPDANCE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_STARLIGHT);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE);
}

bool SubCharactor::init(CCPoint positoiin)
{
    if ( !CCSprite::initWithFile("Sub_h_90.png") ) {
        return false;
    }
    basePos=positoiin;
    this->setPosition(positoiin);
    
    CCArray* ary = CCArray::create();
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0001.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0002.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0003.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0004.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0005.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0006.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0007.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0008.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0009.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0010.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0011.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0012.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0013.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0014.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0015.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0016.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0017.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0018.png")->getCString());
    ary->addObject(frame);
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("Sub_0019.png")->getCString());
    ary->addObject(frame);
    
    CCAnimation* subCharaBaseAnimation = CCAnimation::createWithSpriteFrames(ary);
    subCharaBaseAnimation->setDelayPerUnit(0.03);
    CCAnimate * animate = CCAnimate::create(subCharaBaseAnimation);
    runAction(CCRepeatForever::create(animate));
    
    changeAnimation(SubCharactorStatus::Status_Normal);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationRollingAfter), NOTIFICATION_SUBCHARACTOR_ANIMATION_ROLLING, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationJump), NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationJumpCheck), NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP_CHECK, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationJumpDance), NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMPDANCE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationStarLight), NOTIFICATION_SUBCHARACTOR_ANIMATION_STARLIGHT, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(SubCharactor::changeAnimationTutorialPageMove), NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE, NULL);
    
    
    return true;
}


SubCharactor* SubCharactor::create(CCPoint positoiin)
{
    SubCharactor* pRet = new SubCharactor();
    if (pRet && pRet->init(positoiin)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void SubCharactor::changeAnimation(SubCharactorStatus::Status subCharactorStatus)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    switch (subCharactorStatus) {
        case SubCharactorStatus::Status_Normal:
        {
            stopActionByTag(TAG_ANIM_NORMAL);
            status=SubCharactorStatus::Status_Normal;
            CCActionInterval* delay = CCDelayTime::create(14.0);
            CCActionInterval* moveDown = CCMoveBy::create(5.0, ccp(20, -40));
            CCActionInterval* moveUp = CCMoveTo::create(1.0, basePos);
            CCSequence* seq = CCSequence::create(delay,moveDown, moveUp, NULL);
            CCRepeatForever* repeat = CCRepeatForever::create(seq);
            repeat->setTag(TAG_ANIM_NORMAL);
            runAction(repeat);
        }
            break;
        case SubCharactorStatus::Status_Rolling_After:
        {
            if ( status!=SubCharactorStatus::Status_Normal ) {
                return;
            }
            stopActionByTag(TAG_ANIM_NORMAL);
            status=SubCharactorStatus::Status_Rolling_After;

            
            CCMoveBy* move1 = CCMoveBy::create(1.0, ccp(-1000,0));
            CCRotateTo* rotate1 = CCRotateTo::create(1.0, 1800.0);
            CCSpawn* spawn1 = CCSpawn::create(move1, rotate1, NULL);
            
            CCDelayTime* delay = CCDelayTime::create(0.7);
            
            CCMoveTo* move2 = CCMoveTo::create(1.0, basePos);
            CCRotateTo* rotate3 = CCRotateTo::create(1.0, 1800.0);
            CCJumpBy* jump1 = CCJumpBy::create(1.0, CCPointZero, 400, 1);
            CCSpawn* spawn3 = CCSpawn::create(move2, rotate3, jump1, NULL);
            
            CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(SubCharactor::createAngre));
            CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(SubCharactor::changeAnimationNormal));
            
            runAction(CCSequence::create(spawn1, delay, spawn3, CCDelayTime::create(0.2), func1, func2, NULL));
            
            //Vertical Circle
            
            
        }
            break;
        case SubCharactorStatus::Status_Jump:
        case SubCharactorStatus::Status_Jump_Check:
        {
            if ( status!=SubCharactorStatus::Status_Normal ) {
                return;
            }
            stopActionByTag(TAG_ANIM_NORMAL);
            
            status=SubCharactorStatus::Status_Jump;
            
            CCActionInterval* act = NULL;
            
            CCActionInterval* jump = CCJumpBy::create(0.8, CCPointZero, 20, 2);
            
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SubCharactor::changeAnimationNormal));

            
            act = CCSequence::create(jump,func,NULL);
            
            if ( subCharactorStatus==SubCharactorStatus::Status_Jump_Check ) {
                CCActionInterval* jump1 = CCJumpBy::create(0.4, CCPointZero, 20, 1);
                CCActionInterval* jump3 = CCJumpBy::create(0.6, CCPointZero, 60, 1);
                
                CCActionInterval* jump2 = CCJumpBy::create(0.8, CCPointZero, 120, 1);
                CCDelayTime* delay = CCDelayTime::create(0.2);
                CCRotateTo* rotate = CCRotateTo::create(0.6, -1080);
                CCSequence* seq = CCSequence::create(delay, rotate, NULL);
                CCSpawn* spawn = CCSpawn::create(jump2, seq, NULL);
                act = CCSequence::create(jump1, jump3, spawn,func,NULL);
                
            }
            
            runAction(act);
   
        }
            break;
        case SubCharactorStatus::Status_StarLight:
        {
//            if ( status!=SubCharactorStatus::Status_Normal ) {
//                return;
//            }
//            stopActionByTag(TAG_ANIM_NORMAL);
            
//            status=SubCharactorStatus::Status_StarLight;

            
            CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(SubCharactor::animStarLight1));
            
            runAction(CCSequence::create(func1, NULL));
            
        }
            break;
        case SubCharactorStatus::Status_JumpDance:
        {
            if ( status!=SubCharactorStatus::Status_Normal ) {
                return;
            }
            stopActionByTag(TAG_ANIM_NORMAL);
            
            CCPoint nowPos = getPosition();
            float delta = basePos.y - nowPos.y;
            
            status=SubCharactorStatus::Status_JumpDance;
            
            CCCallFunc* reset = CCCallFunc::create(this,callfunc_selector(SubCharactor::changeAnimationNormal));

            
            CCSprite* sprite = CCSprite::createWithSpriteFrameName("nc71054.png");
            sprite->setPosition(ccp(100, 110));
            sprite->setScale(0.3);
            CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
            sprite->runAction(CCSequence::create(CCBlink::create(4, 4), func, NULL));
            addChild(sprite);
            
            sprite = CCSprite::createWithSpriteFrameName("nc71060.png");
            sprite->setPosition(ccp(100, 110));
            sprite->setScale(0.0);
            CCEaseBackOut* scale = CCEaseBackOut::create(CCScaleTo::create(0.4, 0.3));
            func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
            sprite->runAction(CCSequence::create(CCDelayTime::create(6.0), scale, CCDelayTime::create(2.0), func, reset, NULL));
            addChild(sprite);


            
            CCActionInterval* jump1 = CCJumpBy::create(1.0, CCPointZero, 50, 2);
            
            CCActionInterval* jump2 = CCJumpBy::create(1.0, CCPointZero, 120+delta, 1);
            CCActionInterval* move1 = CCMoveBy::create(1.0, ccp(0.0,170.0+delta));
            CCSpawn* spawn1 = CCSpawn::create(jump2, move1, NULL);

            CCActionInterval* jump3 = CCJumpBy::create(1.0, CCPointZero, 100, 1);
            CCActionInterval* move2 = CCMoveBy::create(1.0, ccp(-100.0,-60.0));
            CCActionInterval* rotate1 = CCRotateBy::create(1.0, -360);
            CCSpawn* spawn2 = CCSpawn::create(jump3, move2, rotate1 ,NULL);

            CCActionInterval* jump4 = CCJumpBy::create(1.0, CCPointZero, 100, 1);
            CCActionInterval* move3 = CCMoveBy::create(1.0, ccp(150.0,-60.0));
            CCActionInterval* rotate2 = CCRotateBy::create(1.0, 360);
            CCSpawn* spawn3 = CCSpawn::create(jump4, move3, rotate2 ,NULL);

            CCActionInterval* jump5 = CCJumpBy::create(1.0, CCPointZero, 100, 1);
            CCActionInterval* move4 = CCMoveTo::create(1.0, basePos);
            CCActionInterval* rotate3 = CCRotateBy::create(1.0, -1080);
            CCSpawn* spawn4 = CCSpawn::create(jump5, move4, rotate3 ,NULL);

            
            
            runAction(CCSequence::create(jump1, spawn1, spawn2, spawn3, spawn4, NULL));
            
        }
            break;
        case SubCharactorStatus::Status_Tutorial_PageMove:
        {
//            CCSprite* finger = CCSprite::createWithSpriteFrameName("Finger.png");
//            addChild(finger, 0, TAG_ANIM_FINGER);
//            finger->setPosition(ccp(5,110));
            status=SubCharactorStatus::Status_Tutorial_PageMove;
//            CCPoint point = CCPointMake(basePos.x-170, basePos.y-70);
//            CCActionInterval* move1 = CCMoveTo::create(1.5, point);
//            CCActionInterval* delay = CCDelayTime::create(2.0);
//            CCActionInterval* move2 = CCMoveTo::create(1.5, basePos);
            CCActionInterval* scale1 = CCScaleTo::create(0.2, 0.0);
            CCEaseBackIn* ease1 = CCEaseBackIn::create(scale1);
            CCActionInterval* delay1 = CCDelayTime::create(1.0);
            CCActionInterval* scale2 = CCScaleTo::create(0.2, 1.0);
            CCEaseBackOut* ease2 = CCEaseBackOut::create(scale2);
            CCActionInterval* delay2 = CCDelayTime::create(4.0);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SubCharactor::changeAnimationNormal));
            runAction(CCSequence::create(ease1, delay2, ease2, func, NULL));
            
        }
            break;
        default:
            break;
    }
}

void SubCharactor::createAngre()
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("nc71072.png");
    sprite->setPosition(ccp(100, 110));
    sprite->setScale(0.3);
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCBlink::create(0.8, 2), func, NULL));
    addChild(sprite);
    
}

void SubCharactor::animStarLight1()
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("nc71068.png");
    sprite->setPosition(ccp(100, 110));
    sprite->setScale(0.0);
    CCEaseBackOut* scale = CCEaseBackOut::create(CCScaleTo::create(0.4, 0.3));
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCDelayTime::create(2.0), scale, CCDelayTime::create(2.0) ,func, NULL));
    addChild(sprite);
    
    sprite = CCSprite::createWithSpriteFrameName("nc71069.png");
    sprite->setPosition(ccp(100, 110));
    sprite->setScale(0.0);
    CCEaseBackOut* scale2 = CCEaseBackOut::create(CCScaleTo::create(0.4, 0.3));
    CCCallFunc* func2 = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCCallFunc* func3 = CCCallFunc::create(this, callfunc_selector(SubCharactor::changeAnimationNormal));
    
    sprite->runAction(CCSequence::create(CCDelayTime::create(17.0), scale2, CCDelayTime::create(2.0) ,func3, func2, NULL));
    
    addChild(sprite);
    
}
