//
//  Charactor.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/05.
//
//

#include "Charactor.h"
#include "Book.h"
#include "CommonDefine.h"
#include "ConvergenceEffect.h"

Charactor::Charactor()
{
    status=CharactorStatus::Status_Normal;
    animation=CharactorStatus::AnimationStatus_Fly;
}


Charactor::~Charactor()
{
    status=CharactorStatus::Status_Normal;
    animation=CharactorStatus::AnimationStatus_Fly;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTOR_SUBEFFECT_NONE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_IMAGE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_CANCEL);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_COMPLETE);

}

bool Charactor::init(const char *filename)
{
    if ( !CCSprite::initWithFile(filename) ) {
        return false;
    }
    changeAnimation();
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Charactor::addSubEffectNone), NOTIFICATION_CHARACTOR_SUBEFFECT_NONE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Charactor::addSubEffectSearch), NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Charactor::addSubEffectSearchImage), NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_IMAGE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Charactor::addSubEffectSearchCancel), NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_CANCEL, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(Charactor::addSubEffectSearchComplete), NOTIFICATION_CHARACTOR_SUBEFFECT_SEARCH_COMPLETE, NULL);

    
    CCNode* starPos = CCNode::create();
    starPos->setPosition(ccp(getContentSize().width/2.0+60, getContentSize().height/2.0-75));
    addChild(starPos, 0,TAG_CHARACTOR_STARPOS);
    
    return true;
}

Charactor* Charactor::create(const char *filename)
{
    Charactor* pRet = new Charactor();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void Charactor::changeAnimation()
{
    switch (animation) {
        case CharactorStatus::AnimationStatus_Fly:
        {
            this->stopAllActions();
            CCActionInterval* moveDown = CCMoveBy::create(0.5, ccp(0, -20));
            CCActionInterval* moveUp = CCMoveBy::create(1.0, ccp(0, 20));
            CCSequence* seq = CCSequence::create(moveDown, moveUp, NULL);
            this->runAction(CCRepeatForever::create(seq));
        }
            break;
            
        default:
            break;
    }
}

void Charactor::addSubAnimation(CharactorStatus::AnimationStatus animationStatus)
{
    if ( animation!=CharactorStatus::AnimationStatus_Fly ) {
        return;
    }
    animation=animationStatus;
    CCLog("Start Sub Animation");
    switch (animationStatus) {
        case CharactorStatus::AnimationStatus_Rolling:
        {
            this->setAnchorPoint(ccp(0.5,1.0));
            this->setPosition(ccp(getPosition().x, getPosition().y+getContentSize().height/2.0));
            CCActionInterval* rotate0 = CCRotateBy::create(0.5, -10.0, -10.0); //-10
            CCActionInterval* rotate1 = CCRotateBy::create(0.5, 40.0, 40.0);   //+30
            CCActionInterval* rotate2 = CCRotateBy::create(0.5, -70.0, -70.0);  //-40
            CCActionInterval* rotate3 = CCRotateBy::create(0.5, 80.0, 80.0);    //+40
            CCActionInterval* rotate4 = CCRotateBy::create(0.5, -90.0, -90.0);  //-50
            CCActionInterval* rotate5 = CCRotateBy::create(0.5, 410.0, 410.0);  //360
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Charactor::resetAnimationStatus));
            CCDelayTime* delay = CCDelayTime::create(0.05);
            CCEaseSineInOut* act1 = CCEaseSineInOut::create(rotate1);
            CCEaseSineInOut* act2 = CCEaseSineInOut::create(rotate2);
            CCEaseSineInOut* act3 = CCEaseSineInOut::create(rotate3);
            CCEaseSineInOut* act4 = CCEaseSineInOut::create(rotate4);
            CCEaseSineInOut* act5 = CCEaseSineInOut::create(rotate5);
            
            
            this->runAction(CCSequence::create(rotate0,act1,delay,act2,delay,act3,delay,act4,delay,act5,func,NULL));
        }
            break;
        case CharactorStatus::AnimationStatus_StarLight:
        {
            //サブキャラのアニメーション開始
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_STARLIGHT);
            CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(Charactor::animStarLight1));

            this->runAction(CCSequence::create(func1, NULL));
        }
            break;
        case CharactorStatus::AnimationStatus_JumpDance:
        {
            //サブキャラのアニメーション開始
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMPDANCE);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE);

            CCActionInterval* jump1 = CCJumpBy::create(1.0, CCPointZero, 150, 1);
            CCEaseBackIn* jump2 = CCEaseBackIn::create(jump1);
            CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Charactor::resetAnimationStatus));
            runAction(CCSequence::create(jump2, CCDelayTime::create(9.0), func, NULL));
            
        }
            break;
        case CharactorStatus::AnimationStatus_Tap:
        {
        }
            break;
        case CharactorStatus::AnimationStatus_ChangeDetail:
        {
            animChangeDetail();
        }
            break;
        case CharactorStatus::AnimationStatus_Paropunte:
        {
            animParopunte1();
        }
            break;
        default:
            break;
    }
}

void Charactor::addSubEffect(CharactorStatus::SubEffect subEffect)
{
    CCNode* subEffectNode = getChildByTag(TAG_CHARACTOR_EUBEFFECT);
    if ( subEffectNode!=NULL ) {
        removeChildByTag(TAG_CHARACTOR_EUBEFFECT);
    }
    switch (subEffect) {
        case CharactorStatus::SubEffect_Search:
        {
            CCNode* node = CCNode::create();
            CCSprite* search = CCSprite::createWithSpriteFrameName("img_common_search.png");
            node->addChild(search);
            search->runAction(CCRepeatForever::create(CCBlink::create(1.5, 1)));
            addChild(node, 0, TAG_CHARACTOR_EUBEFFECT);
            CCSize size = getContentSize();
            node->setPosition(ccp(size.width-search->getContentSize().width/2.0-80, size.height));
        }
            break;
        case CharactorStatus::SubEffect_Search_Image:
        {
            CCNode* node = CCNode::create();
            CCSprite* search = CCSprite::createWithSpriteFrameName("img_common_search_getimage.png");
            node->addChild(search);
            search->runAction(CCRepeatForever::create(CCBlink::create(1.5, 1)));
            addChild(node, 0, TAG_CHARACTOR_EUBEFFECT);
            CCSize size = getContentSize();
            node->setPosition(ccp(size.width-search->getContentSize().width/2.0-80, size.height));
        }
            break;

        case CharactorStatus::SubEffect_Search_complete:
        {
            CCNode* node = CCNode::create();
            CCSprite* search = CCSprite::createWithSpriteFrameName("img_common_search_complete.png");
            node->addChild(search);
            addChild(node, 0, TAG_CHARACTOR_EUBEFFECT);
            CCSize size = getContentSize();
            node->setPosition(ccp(size.width-search->getContentSize().width/2.0-80, size.height));
        }
            break;
        case CharactorStatus::SubEffect_Search_Cancel:
        {
            CCNode* node = CCNode::create();
            CCSprite* search = CCSprite::createWithSpriteFrameName("img_common_search_cancel.png");
            node->addChild(search);
            addChild(node, 0, TAG_CHARACTOR_EUBEFFECT);
            CCSize size = getContentSize();
            node->setPosition(ccp(size.width-search->getContentSize().width/2.0-80, size.height));
        }
            break;
        default:
            break;
    }
}

void Charactor::resetAnimationStatus()
{
    switch (animation) {
        case CharactorStatus::AnimationStatus_Rolling:
        {
            setAnchorPoint(ccp(0.5,0.5));
            this->setPosition(ccp(getPosition().x, getPosition().y-getContentSize().height/2.0));
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_ROLLING);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE);

        }
            break;
        case CharactorStatus::AnimationStatus_StarLight:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_ROLLING);
//            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_TUTORIAL_PAGEMOVE);
        }
        default:
            break;
    }
    CCLOG("Change AnimationStatus_Fly");
    animation = CharactorStatus::AnimationStatus_Fly;
}

void Charactor::animStarLight1()
{
    ConvergenceEffectLayer* layer = ConvergenceEffectLayer::create();
    CCPoint point = CCPointMake(50, 200);
    layer->setCentetPos(point);
    layer->setLifeTIme(10);
    layer->go();
    addChild(layer);
    
    
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("free_comment_2.png");
    CCSize size = getContentSize();
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0-30, size.height));
    sprite->setScale(0.0);
    CCEaseBackOut* scale = CCEaseBackOut::create(CCScaleTo::create(0.3, 1.0));
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(scale, CCDelayTime::create(4.5), func, NULL));
    addChild(sprite);

    sprite = CCSprite::createWithSpriteFrameName("free_comment_1.png");
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0-30, size.height));
    sprite->setScale(0.0);
    CCEaseBackOut* scale2 = CCEaseBackOut::create(CCScaleTo::create(0.3, 1.0));
    CCCallFunc* func1 = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCDelayTime::create(5.0), scale2, CCDelayTime::create(4.5), func1,NULL));
    addChild(sprite);

    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Charactor::animStarLight2));
    runAction(CCSequence::create(CCDelayTime::create(16.0), func2, NULL));

}

void Charactor::animStarLight2()
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("nc71064.png");
    CCSize size = getContentSize();
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0, size.height));
    sprite->setScale(0.5);
    addChild(sprite);
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Charactor::animStarLight3));
    runAction(CCSequence::create(CCDelayTime::create(4.0), func, func2, NULL));

}

void Charactor::animStarLight3()
{
    CCRotateBy* rotate = CCRotateBy::create(0.3, 0.0,360);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Charactor::resetAnimationStatus));

    runAction(CCSequence::create(rotate, func, NULL));
}

void Charactor::animChangeDetail()
{
    const char* filename = "";
    int file = arc4random() % 3;
    if ( file==0 ) {
        filename = "mahoujin_cyan_100.gif";
    } else if ( file==1 ) {
        filename = "mahoujin_green_100.gif";
    } else {
        filename = "mahoujin_pink_100.gif";
    }
//    CCSprite* star = CCSprite::createWithSpriteFrameName(filename);
    CCSprite* star = CCSprite::create(filename);
    star->setScale(0.0);
    star->setPosition(ccp(50,210));
    addChild(star);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Charactor::resetAnimationStatus));
    CCCallFunc* func2 = CCCallFunc::create(star, callfunc_selector(CCSprite::removeFromParent));
    
    CCActionInterval* act1 = CCSequence::create(CCScaleTo::create(0.2, 1.0), CCDelayTime::create(0.3),CCScaleTo::create(0.5, 3.0), NULL);
    CCSpawn* act2 = CCSpawn::create(act1, CCRotateBy::create(1.0, 360), NULL);

    star->runAction(CCSequence::create(act2,func, func2, NULL));
    
}

void Charactor::animParopunte1()
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("hukidashi_maru.png");
    sprite->setScale(0.6);
    sprite->setPosition(ccp(0,280));
    CCActionInterval* move1 = CCMoveBy::create(0.0, ccp(20, 50));
    CCActionInterval* move2 = CCMoveBy::create(0.0, ccp(-20, -50));
    CCActionInterval* move3 = CCMoveBy::create(0.0, ccp(20, 50));
    CCActionInterval* move4 = CCMoveBy::create(0.0, ccp(-20, -50));
    CCActionInterval* move5 = CCMoveBy::create(0.0, ccp(20, 50));
    CCActionInterval* delay1 = CCDelayTime::create(1.2);
    CCActionInterval* delay2 = CCDelayTime::create(1.2);
    CCActionInterval* delay3 = CCDelayTime::create(1.2);
    CCActionInterval* delay4 = CCDelayTime::create(1.2);
    CCActionInterval* delay5 = CCDelayTime::create(1.2);
    CCActionInterval* delay6 = CCDelayTime::create(1.2);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(Charactor::animParopunte2));
    CCCallFunc* func2 = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCSequence* seq = CCSequence::create(delay5, move1, delay1, move2, delay2, move3, delay3, func, func2 ,NULL);
    sprite->runAction(seq);
    addChild(sprite);
    
    CCSprite* sprite2 = CCSprite::createWithSpriteFrameName("hukidashi_sowa.png");
    sprite2->setPosition(ccp(sprite->getContentSize().width/2.0, sprite->getContentSize().height/2.0));
    sprite->addChild(sprite2);
}

void Charactor::animParopunte2()
{
    CCSize size = getContentSize();

    CCSprite* sprite = CCSprite::createWithSpriteFrameName("nc71060.png");
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0, size.height));
    sprite->setScale(0.0);
    CCEaseBackOut* scale = CCEaseBackOut::create(CCScaleTo::create(0.3, 0.5));
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCDelayTime::create(2.0),scale, CCDelayTime::create(2.0) ,func, NULL));
    addChild(sprite);

    
    sprite = CCSprite::createWithSpriteFrameName("free_comment_6.png");
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0-30, size.height));
    sprite->setScale(0.0);
    scale = CCEaseBackOut::create(CCScaleTo::create(0.3, 1.0));
    func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCDelayTime::create(6.0) ,scale, CCDelayTime::create(4.5), func, NULL));
    addChild(sprite);

    sprite = CCSprite::createWithSpriteFrameName("free_comment_7.png");
    sprite->setPosition(ccp(size.width-sprite->getContentSize().width/2.0-30, size.height));
    sprite->setScale(0.0);
    CCEaseBackOut* scale2 = CCEaseBackOut::create(CCScaleTo::create(0.3, 1.0));
    CCCallFunc* func1 = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(CCDelayTime::create(11.0), scale2, CCDelayTime::create(4.5), func1,NULL));
    addChild(sprite);
    
    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(Charactor::resetAnimationStatus));

    runAction(CCSequence::create(CCDelayTime::create(16.0), func2, NULL));
    

    
}