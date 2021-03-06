//
//  CharactorLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/05.
//
//

#include "CharactorLayer.h"
#include "StarFlyEffect.h"
#include "SubCharactor.h"
#include "CommonDefine.h"
#include "ConvergenceEffect.h"

CharactorLayer::CharactorLayer()
{
    animationStatus=CharactorStatus::AnimationStatus_Fly;
    totalDelta=0.0;
    StarFlyEffect* star = StarFlyEffect::create();
    star->getTexture()->generateMipmap();
    ccTexParams texParams = {GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,
        GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
    star->getTexture()->setTexParameters(&texParams);
    charaNode=NULL;
    isSmall=false;
    isMove=true;
    CharactorBasePos = ccp(530,650);
}

CharactorLayer::~CharactorLayer()
{
    animationStatus=CharactorStatus::AnimationStatus_Fly;
    totalDelta=0.0;
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTORLAYER_EDIT_MAGICAL_CIRCLE);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_CHARACTORLAYER_RESET_TOTAL_DELTA);
    charaNode=NULL;
    isSmall=false;
    isMove=true;
    CharactorBasePos = ccp(530,650);
}

bool CharactorLayer::init()
{
    if ( !CCLayer::init() ) {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CharactorBasePos = ccp(winSize.width/2.0+210, winSize.height/2.0+170);
    
    charaNode = CCNode::create();
    charaNode->setPosition(CharactorBasePos);
    addChild(charaNode);
    
    initCharactor();
    initSubCharactor();
    initBook();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CharactorLayer::editMagicalCircle), NOTIFICATION_CHARACTORLAYER_EDIT_MAGICAL_CIRCLE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(CharactorLayer::resetTatalDelta), NOTIFICATION_CHARACTORLAYER_RESET_TOTAL_DELTA, NULL);
    
    schedule(schedule_selector(CharactorLayer::createStar), 0.5);
    scheduleUpdate();
    setTouchEnabled(true);
    setTouchPriority(0);
    return true;
}

void CharactorLayer::initCharactor()
{
    Charactor* charactor =(Charactor*)charaNode->getChildByTag(TAG_CHARACTOR);
    if ( charactor!=NULL) {
        CCImage *image = new CCImage();
        bool flg =image->initWithImageFile(IMG_FORMAT);
        if ( !flg ) {
            return;
        }
        image->autorelease();
        CCTexture2D *texture = new CCTexture2D();
        texture->initWithImage(image);
        texture->autorelease();
        charactor->setTexture(texture);
        return;
    }
    charactor = Charactor::create(IMG_FORMAT);
    charaNode->addChild(charactor, ZORDET_CHARACTRO, TAG_CHARACTOR);
}

void CharactorLayer::initSubCharactor()
{
    Charactor* charactor =(Charactor*)charaNode->getChildByTag(TAG_CHARACTOR);
    if ( charactor==NULL) {
        return;
    }
    
    SubCharactor* subCharactor = SubCharactor::create(ccp(160,210));
    
    charactor->addChild(subCharactor, 0, TAG_SUB_CHARACTOR);
}

void CharactorLayer::initBook()
{
    createBook("book1.png", -3.5, 0, ZORDER_BOOK, 100, 0, 0.0, -150);
    createBook("book2.png", -3.0, 0, ZORDER_BOOK, 70, 0, 30, -160);
    createBook("book3.png", -3.2, 0, ZORDER_BOOK, 80, 0, 40, -170);
    createBook("book4.png", 3.2, 0, ZORDER_BOOK, 80, 0, 40, -170);
}

Book* CharactorLayer::createBook(const char* fileName, float speed, int deg, int zOrder, float widthX, float widthY, float basePosX, float basePosY) {
    Book* book = Book::create(fileName);
    book->setSpeed(speed);
    book->setDelayDeg(deg);
    book->setZOrderSetting(zOrder);
    book->setWidthX(widthX);
    book->setWidthY(widthY);
    book->setBasePosX(basePosX);
    book->setBasePosY(basePosY);
    charaNode->addChild(book, ZORDER_BOOK, TAG_BOOKS);
    return book;
}

void CharactorLayer::initStarFlayEffect()
{
    
}

void CharactorLayer::createStar()
{
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    
    SubCharactor* sub = dynamic_cast<SubCharactor*>(charactor->getChildByTag(TAG_SUB_CHARACTOR));
    if ( sub==NULL ) {
        return;
    }
    
    StarFlyEffect* star = StarFlyEffect::create();
    star->getTexture()->generateMipmap();
    ccTexParams texParams = {GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR,
        GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE};
    star->getTexture()->setTexParameters(&texParams);

    star->setPosition(ccp(sub->getPosition().x+15,sub->getPosition().y+15));
    star->setScale(0.0);
    star->initActionRundom(arc4random());
    
    charactor->addChild(star,1);
    
}

void CharactorLayer::moveLayer(cocos2d::CCPoint point, float duration)
{
    if ( isSmall ) {
        return;
    }
    if ( !isMove ) {
        return;
    }
    runAction(CCMoveBy::create(duration, point));
}

void CharactorLayer::createMagicCircleEffect(MagicCircleEffectStatus::ColorType color, MagicCircleEffectStatus::StarMoveType starMove)
{
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    MagicCircleEffect* effect = MagicCircleEffect::create(color, MagicCircleEffectStatus::AnimationType_Normal, starMove);
    effect->setPosition(ccp(40.0,215.0));
    charactor->addChild(effect, ZORDET_CHARACTRO);
    if ( MagicCircleEffectStatus::ColorType_NONE!=color ) {
        switch (starMove) {
            case MagicCircleEffectStatus::StarMoveType_IN:
                createParticleEffect(0.3);
                break;
            default:
                createParticleEffect(0.2);
                break;
        }
    }
    resetTatalDelta();
    addSubAnimation(CharactorStatus::AnimationStatus_Rotate);
}

void CharactorLayer::createParticleEffect(float duration)
{
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    CCNode* node = CCNode::create();
    charactor->addChild(node,ZORDET_CHARACTRO);
    
    node->setPosition(ccp(40.0,215.0));
    
    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("ListChangeEffect.plist");
    particle->setTotalParticles(150);
    particle->setPosition(CCPointZero);
    particle->setDuration(duration);
    particle->setAutoRemoveOnFinish(true);
    node->addChild(particle, ZORDET_CHARACTRO);
    CCCallFunc* func = CCCallFunc::create(node, callfunc_selector(CCNode::removeFromParent));
    node->runAction(CCSequence::create(CCDelayTime::create(2.0), func, NULL));
}

void CharactorLayer::resetPosition()
{
    if ( isSmall ) {
        return;
    }
    if ( !isMove ) {
        return;
    }

    runAction(CCMoveTo::create(0.5, ccp(0,0)));
}

void CharactorLayer::addSubAnimation(CharactorStatus::AnimationStatus animation)
{
    CCLog("Start Sub Animation onLayer");
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    
    CCSprite* sub = dynamic_cast<CCSprite*>( charactor->getChildByTag(TAG_SUB_CHARACTOR) );
    if ( sub==NULL ) {
        return;
    }
    
    if ( animationStatus!=CharactorStatus::AnimationStatus_Fly ) {
        return;
    }
    
    
    
    animationStatus = animation;
    
    switch (animation) {
        case CharactorStatus::AnimationStatus_Rotate_left:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP_CHECK);
            animationStatus = CharactorStatus::AnimationStatus_Fly;

            MagicCircleEffect* effect = MagicCircleEffect::create(MagicCircleEffectStatus::ColorType_NONE2, MagicCircleEffectStatus::AnimationType_TYPE1, MagicCircleEffectStatus::StarMoveType_OUT);
            effect->setPosition(ccp(40.0,215.0));
            createParticleEffect(0.1);

            charactor->addChild(effect, ZORDET_CHARACTRO);

            resetTatalDelta();
        }
            break;
        case CharactorStatus::AnimationStatus_Rotate_right:
        {
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_SUBCHARACTOR_ANIMATION_JUMP);
            animationStatus = CharactorStatus::AnimationStatus_Fly;
            MagicCircleEffect* effect = MagicCircleEffect::create(MagicCircleEffectStatus::ColorType_NONE2, MagicCircleEffectStatus::AnimationType_TYPE1, MagicCircleEffectStatus::StarMoveType_OUT);
            effect->setPosition(ccp(40.0,215.0));
            createParticleEffect(0.1);

            charactor->addChild(effect, ZORDET_CHARACTRO);
            resetTatalDelta();
        }
            break;
        case CharactorStatus::AnimationStatus_Rotate:
        {
            animationStatus = CharactorStatus::AnimationStatus_Fly;
        }
            break;
        case CharactorStatus::AnimationStatus_Tap:
        {
            animationStatus = CharactorStatus::AnimationStatus_Fly;

            Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
            if ( charactor==NULL ) {
                return;
            }
            
            CCLOG("%s", "Start Charactor Animation");
//            charactor->addSubAnimation(CharactorStatus::AnimationStatus_Paropunte);
//
//            return;
            if ( isSmall==true ) {
                CCActionInterval* act1 = CCScaleTo::create(0.3, 1.0);
                CCEaseBackOut* act2 = CCEaseBackOut::create(act1);
                CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CharactorLayer::resetAnimationStatus));
                charaNode->runAction(CCSequence::create(act2,func,NULL));
                isSmall=false;
                moveLayer(ccp(-70.0,0), 0.3);
            } else {
            
                CCActionInterval* act1 = CCScaleTo::create(0.3, 0.5);
                CCEaseBackOut* act2 = CCEaseBackOut::create(act1);
                CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CharactorLayer::resetAnimationStatus));
                charaNode->runAction(CCSequence::create(act2,func,NULL));
                moveLayer(ccp(70.0,0), 0.3);
                isSmall=true;
            }
//            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_LAYER_CONTROLL_DETAIL_CAMERA);

            
        }
            
        default:
            break;
    }

}

void CharactorLayer::addSubEffect(CharactorStatus::SubEffect subEffect)
{
    CCLog("Start Sub Animation onLayer");
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    
    charactor->addSubEffect(subEffect);
}

void CharactorLayer::resetAnimationStatus()
{
    animationStatus=CharactorStatus::AnimationStatus_Fly;
}

void CharactorLayer::editMagicalCircle()
{
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    CCLayer* layer = CCLayer::create();
    
    CCNode* node = CCNode::create();
    node->setPosition(ccp(CharactorBasePos.x, CharactorBasePos.y-400));
    layer->addChild(node);
    
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("bg_mahoujin_overray_1.png");
    sprite->setScale(0.8);
    node->addChild(sprite);
    
    
    sprite = CCSprite::create("mahoujin_pink.gif");
    sprite->setScale(0.6);
    node->addChild(sprite);
    node->runAction(CCRepeatForever::create(CCRotateBy::create(3.0, 360)));
    
    float x=0, y=0, z=0;
    layer->getCamera()->getCenterXYZ(&x, &y, &z);
    layer->getCamera()->setCenterXYZ(x, y+0.0000003, z);
    addChild(layer);
    
    node->setScale(0.0);
    
    CCScaleTo* scale1 = CCScaleTo::create(0.3, 1.0);
    CCScaleTo* scale2 = CCScaleTo::create(1.0, 0.0);
    CCCallFunc* remove = CCCallFunc::create(layer, callfunc_selector(CCLayer::removeFromParent));
    node->runAction(CCSequence::create(scale1, CCDelayTime::create(11.5), scale2, remove, NULL));
}

void CharactorLayer::update(float delta)
{
    totalDelta+=delta;
    if ( totalDelta>40.0 ) {
        doRandamAnimation();
        resetTatalDelta();
    }
}

void CharactorLayer::doRandamAnimation()
{
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }
    
    CCLOG("%s", "Start Charactor Animation");
    
    int random = arc4random() % 100;
    if ( 0<=random && 45 >random) {
        charactor->addSubAnimation(CharactorStatus::AnimationStatus_Rolling);
        return;
    }
    if ( 45<=random && 85>random ) {
        charactor->addSubAnimation(CharactorStatus::AnimationStatus_JumpDance);
        return;
    }
    charactor->addSubAnimation(CharactorStatus::AnimationStatus_StarLight);

}

void CharactorLayer::resetTatalDelta()
{
    totalDelta=0.0;
}

void CharactorLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool CharactorLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = convertTouchToNodeSpace(pTouch);
    CCSize charaSize = CCSizeMake(480, 480);
    int minX = charaNode->getPosition().x - charaSize.width/4.0;
    int maxX = charaNode->getPosition().x + charaSize.width/4.0;
    if ( minX>point.x ) {
        return false;
    }
    if ( maxX<point.x ) {
        return false;
    }
    int minY = charaNode->getPosition().y;
    int maxY = charaNode->getPosition().y + charaSize.height/4.0;
    if ( minY>point.y ) {
        return false;
    }
    if ( maxY<point.y ) {
        return false;
    }
    return true;
}

void CharactorLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = convertTouchToNodeSpace(pTouch);
    CCSize charaSize = CCSizeMake(480, 480);
    int minX = charaNode->getPosition().x - charaSize.width/4.0;
    int maxX = charaNode->getPosition().x + charaSize.width/4.0;
    if ( minX>point.x ) {
        return;
    }
    if ( maxX<point.x ) {
        return;
    }
    int minY = charaNode->getPosition().y - charaSize.height/4.0;
    int maxY = charaNode->getPosition().y + charaSize.height/4.0;
    if ( minY>point.y ) {
        return;
    }
    if ( maxY<point.y ) {
        return;
    }
    
    Charactor* charactor = dynamic_cast<Charactor*>(charaNode->getChildByTag(TAG_CHARACTOR));
    if ( charactor==NULL ) {
        return;
    }

    addSubAnimation(CharactorStatus::AnimationStatus_Tap);
}

void CharactorLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    
}

void CharactorLayer::startStarLightBreaker()
{
    initSLB();
    
}

void CharactorLayer::initSLB()
{
    isSLB=TRUE;
    deltaSLB=0.0;
    totalSLBSize = 10;
    schedule(schedule_selector(CharactorLayer::updateSLB));
    stars = CCArray::create();
}

void CharactorLayer::updateSLB()
{
    
}
