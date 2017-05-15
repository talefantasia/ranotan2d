//
//  MagicCircleEffect.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/20.
//
//

#include "MagicCircleEffect.h"
#include "AnimationFactory.h"
#include <stdlib.h>

MagicCircleEffect::MagicCircleEffect()
{
    color=MagicCircleEffectStatus::ColorType_GREEN;
    animation=MagicCircleEffectStatus::AnimationType_Normal;
    starMove=MagicCircleEffectStatus::StarMoveType_OUT;
}

MagicCircleEffect::~MagicCircleEffect()
{
    color=MagicCircleEffectStatus::ColorType_GREEN;
    animation=MagicCircleEffectStatus::AnimationType_Normal;
    starMove=MagicCircleEffectStatus::StarMoveType_OUT;
    CCLOG("%s", "~MagicCircleDestractor");
}

MagicCircleEffect* MagicCircleEffect::create(MagicCircleEffectStatus::ColorType color, MagicCircleEffectStatus::AnimationType animation, MagicCircleEffectStatus::StarMoveType starMoveType)
{
    MagicCircleEffect* pRet = new MagicCircleEffect();
    pRet->runAction(CCSequence::create(CCDelayTime::create(6.0), CCCallFunc::create(pRet, callfunc_selector(MagicCircleEffect::removeFromParent)), NULL));

    if (pRet && pRet->init(color, animation, starMoveType)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

bool MagicCircleEffect::init(MagicCircleEffectStatus::ColorType colorType, MagicCircleEffectStatus::AnimationType animationType, MagicCircleEffectStatus::StarMoveType starMoveType)
{
    if ( !CCNode::create() ) {
        return false;
    }
    int deg = arc4random() % 360;
    setRotation(deg);
    color = colorType;
    animation=animationType;
    starMove=starMoveType;
    initCircleEffect();
    initStarEffect();
    return true;
}

void MagicCircleEffect::initCircleEffect()
{
    switch (animation) {
        case MagicCircleEffectStatus::AnimationType_Normal:
        {
            CCSprite* sprite = CCSprite::createWithSpriteFrameName("ring_circle.png");
            sprite->setPosition(ccp(0.0,0.0));
            sprite->setScale(0);
            CCString* actionName = CCString::create("detail-content-notice-4");
            if (color==MagicCircleEffectStatus::ColorType_NONE) {
                actionName = CCString::create("detail-content-notice-6");
            }
            CCActionInterval* action1 = AnimationFactory::sharedInstance()->getActions(actionName->getCString(),CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)));
            CCCallFunc::create(sprite, callfunc_selector(CCNode::removeFromParent));
            addChild(sprite);
            sprite->runAction(action1);

        }
            break;
        case MagicCircleEffectStatus::AnimationType_TYPE1:
        {
            CCSprite* sprite = CCSprite::createWithSpriteFrameName("ring_circle.png");
            sprite->setPosition(ccp(0.0,0.0));
            sprite->setScale(0);
            CCString* actionName = CCString::create("detail-content-notice-8");
            CCActionInterval* action1 = AnimationFactory::sharedInstance()->getActions(actionName->getCString(),CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)));
            CCCallFunc::create(sprite, callfunc_selector(CCNode::removeFromParent));
            addChild(sprite);
            sprite->runAction(action1);
            
        }
            break;
            
        default:
            break;
    }
}

void MagicCircleEffect::initStarEffect()
{
    const char* filename;
    switch (color) {
        case MagicCircleEffectStatus::ColorType_GREEN:
            filename = "img_star_green2.png";
            break;
        case MagicCircleEffectStatus::ColorType_PURPLE:
            filename = "img_star_purple.png";
            break;
        case MagicCircleEffectStatus::ColorType_BLUE:
            filename = "img_star_blue.png";
            break;
        case MagicCircleEffectStatus::ColorType_NONE:
        case MagicCircleEffectStatus::ColorType_NONE2:
        {
            CCNode* node = CCNode::create();
            addChild(node, 0, 10000);
            if ( color==MagicCircleEffectStatus::ColorType_NONE ) {
                node->runAction(CCRotateBy::create(5.0, 1080));
            }
            int startDeg = arc4random() % 360;
            for (int i=0; i<20; i++) {
                int file = arc4random() % 3;
                if ( file==0 ) {
                    filename = "img_star_green2.png";
                } else if ( file==1 ) {
                    filename = "img_star_purple.png";
                } else {
                    filename = "img_star_blue.png";
                }
                int deg = startDeg + 35*i;
                //   CCLOG("%d", deg);
                float r =  100.0;
                if ( color==MagicCircleEffectStatus::ColorType_NONE ) {
                    r =  200.0;
                }
                float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
                float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
                
                
//                int delay1 = arc4random() % 10;
                float delay1 = 0.02*(float)i;
                createStar2(filename, "detail-content-notice-7", ccp(startPosX,startPosY), delay1);
            }
            
        }
            return;
            break;
        default:
        {
//            CCNode* node = CCNode::create();
//            addChild(node, 0, 10000);
//            for (int i=0; i<20; i++) {
//                int file = arc4random() % 3;
//                if ( file==0 ) {
//                    filename = "img_star_green2.png";
//                } else if ( file==1 ) {
//                    filename = "img_star_purple.png";
//                } else {
//                    filename = "img_star_blue.png";
//                }
//                int deg = arc4random() % 360;
//                //   CCLOG("%d", deg);
//                float r = arc4random() % 75 + 150;
//                float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
//                float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
//                
//                
//                int delay1 = arc4random() % 10;
//                createStar(filename, "detail-content-notice-7", ccp(startPosX,startPosY), (float)delay1/10.0);
//            }
//            
        }
            return;
            break;

    }
    
    int forMax= 20;
    int deltaBase = 75;
    float rBase = 200.0;
    if ( starMove==MagicCircleEffectStatus::StarMoveType_IN ) {
        forMax=15;
        rBase = 250.0;
        deltaBase = 100;
    }
    
    if ( starMove==MagicCircleEffectStatus::StarMoveType_OUT ) {

        for (int i=0; i<forMax; i++) {
            
            
            int deg = arc4random() % 360;
            //   CCLOG("%d", deg);
            float r = arc4random() % deltaBase + rBase;
            float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
            float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
            
            
            int delay1 = arc4random() % 6;
            if (starMove==MagicCircleEffectStatus::StarMoveType_OUT)
            {
                createStar2(filename, "detail-content-notice-3", ccp(startPosX,startPosY), (float)delay1/10.0);
            }else {
                createStar(filename, "detail-content-notice-3-2", ccp(startPosX,startPosY), (float)delay1/10.0);
            }
        }
        
    } else {
        for (int i=0; i<10; i++) {
        int deg = arc4random() % 360;
        //   CCLOG("%d", deg);
        float r = arc4random() % deltaBase + 200;
        float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
        float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
        
        
        int delay1 = arc4random() % 20;
        createStar(filename, "detail-content-notice-3-2", ccp(startPosX,startPosY), (float)delay1/100.0);
        }
        //        for (int i=0; i<forMax; i++) {
//            CCNode* node = CCNode::create();
//            addChild(node, 0, 10001);
//            node->runAction(CCRotateBy::create(5.0, 1080));
//            
//            int deg = i * (360 / forMax);
//            float r = CCDirector::sharedDirector()->getWinSize().width/1.5
//            ;
//            float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
//            float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
//            
//            createStar3(filename, "detail-content-notice-3-3", ccp(startPosX,startPosY), 0.03 * i);
//
//            
//        }
        for (int i=0; i<forMax; i++) {
            CCNode* node = CCNode::create();
            addChild(node, 0, 10001);
            node->runAction(CCRotateBy::create(5.0, 1080));
            
            int deg = i * (360 / forMax);
            float r = CCDirector::sharedDirector()->getWinSize().width/1.5
            ;
            float startPosX = r * cos(CC_DEGREES_TO_RADIANS(deg));
            float startPosY = r * sin(CC_DEGREES_TO_RADIANS(deg));
            
            createStar3(filename, "detail-content-notice-3-3", ccp(startPosX,startPosY), 0.0);
            
            
        }
    }
    
    

}

void MagicCircleEffect::createStar(const char* filename, const char* actionName, CCPoint movePos, float delay)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(filename);
    sprite->setPosition(movePos);
    sprite->setScale(0.6);
    sprite->setOpacity(0);

    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCFloat::create(-movePos.x), "move_posx");
    dic->setObject(CCFloat::create(-movePos.y), "move_posy");
    CCActionInterval* action2 = AnimationFactory::sharedInstance()->getActions(actionName,dic,CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)));
    CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCNode* node = getChildByTag(10000);
    if ( node!=NULL ) {
        node->addChild(sprite);
    } else {
        addChild(sprite);
    }
    
    CCSequence* action = CCSequence::create(CCDelayTime::create(delay), action2, NULL);
    
    sprite->runAction(action);

}

void MagicCircleEffect::createStar2(const char* filename, const char* actionName, CCPoint movePos, float delay)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(filename);
    sprite->setPosition(ccp(0,0));
    sprite->setScale(0);
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCFloat::create(movePos.x), "move_posx");
    dic->setObject(CCFloat::create(movePos.y), "move_posy");
    CCActionInterval* action2 = AnimationFactory::sharedInstance()->getActions(actionName,dic,CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)));
    CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCNode* node = getChildByTag(10000);
    if ( node!=NULL ) {
        node->addChild(sprite);
    } else {
        addChild(sprite);
    }
    
    CCSequence* action = CCSequence::create(CCDelayTime::create(delay), action2, NULL);
    
    sprite->runAction(action);
    
}

void MagicCircleEffect::createStar3(const char* filename, const char* actionName, CCPoint movePos, float delay)
{
    CCSprite* sprite = CCSprite::createWithSpriteFrameName(filename);
    sprite->setPosition(movePos);
    sprite->setScale(0.6);
    sprite->setOpacity(0);
    
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(CCFloat::create(-movePos.x), "move_posx");
    dic->setObject(CCFloat::create(-movePos.y), "move_posy");
    CCActionInterval* action2 = AnimationFactory::sharedInstance()->getActions(actionName,dic,CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)));
    CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    CCNode* node = getChildByTag(10001);
    if ( node!=NULL ) {
        node->addChild(sprite);
    } else {
        addChild(sprite);
    }
    
    CCSequence* action = CCSequence::create(CCDelayTime::create(delay), action2, NULL);
    
    sprite->runAction(action);
    
}

