//
//  SerifLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/15.
//
//

#include "SerifLayer.h"
#include "CommonDefine.h"

SerifLayer::SerifLayer()
{
    totalDelta=0;
    scrolls=0;
    isSerifNow=false;
    scrollStartDelay=1.0;
}

SerifLayer::~SerifLayer()
{
    totalDelta=0;
    scrolls=0;
    isSerifNow=false;
    scrollStartDelay=1.0;
}

bool SerifLayer::init()
{
    if ( !CCLayer::create() ) {
        return false;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    basePos = ccp(winSize.width/2.0-40, winSize.height/2.0-235);
    schedule(schedule_selector(SerifLayer::createSerif));
    return true;
}


void SerifLayer::initLayout()
{
    
    serif = CCSprite::create("bg_therf.png");
    serif->setPosition(basePos);
    addChild(serif);
    
    serifSize = CCSizeMake(serif->getContentSize().width-72, serif->getContentSize().height-10);
    
    CCSprite* chara = CCSprite::create("rect3015_shadow.png");
    chara->setScale(0.8);
    chara->setPosition(ccp(500, serif->getContentSize().height/2.0-50));
    serif->addChild(chara);
    
    serif->setScale(0.0);
    
}

void SerifLayer::setTextString(const char *text)
{

    //文字ラベル作成
    CCLabelTTF* label = CCLabelTTF::create(text, FONT_W6, 24, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    setTextString(label);
    
}

void  SerifLayer::setTextQueue(const char *text)
{
//    queue.push(text);
}

void SerifLayer::setTextString(cocos2d::CCLabelTTF *label)
{
    initLayout();

    label->setDimensions(CCSizeMake(serifSize.width, 0.0));
    label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height+label->getFontSize()*2.0));
    
    if ( label->getContentSize().height<serifSize.height ) {
        label->setDimensions(CCSizeMake(label->getContentSize().width, serifSize.height));
    }
    
    //スクロールビュー作成
    scroll = CCScrollView::create();
    scroll->setContainer(label);
    scroll->setViewSize(serifSize);
    scroll->setPosition(ccp(20.0,10.0));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    serif->addChild(scroll);
    serifContentSize = scroll->getContentSize();
    
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SerifLayer::scheduleUpdate));
    
    serif->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.2, 1.0)),func, NULL));
    
    //スクロール開始位置
    CCPoint startPoint = CCPoint(0.0, serifSize.height - serifContentSize.height);
    scroll->setContentOffset(startPoint);

}

void SerifLayer::removeSerif(float delay)
{
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(CCLayer::removeFromParent));
    
    serif->runAction(CCSequence::create(CCDelayTime::create(delay), CCEaseBackIn::create(CCScaleTo::create(0.3, 0.0)),func, NULL));
}

void SerifLayer::update(float delta)
{
    totalDelta+=delta;
    if ( totalDelta<scrollStartDelay ) {
        return;
    }
    scrolls++;
    float y = (serifSize.height + 0.5*scrolls) - serifContentSize.height;
//    if ( serifSize.height/2.0 < y ) {
    if ( -10 < y ) {
        unscheduleUpdate();
        removeSerif(1.0);
    }
    CCPoint startPoint = CCPoint(0.0, y);
    
    
    
//    scroll->setContentOffsetInDuration(startPoint, 0.4);
    scroll->setContentOffset(startPoint);
}


void SerifLayer::createSerif()
{
    if ( isSerifNow ) {
        return;
    }
//    //キューが空なら抜ける
//    if ( queue.empty() ) {
//        return;
//    }
//    //SerifLayerの準備
//    serifStr = (std::string)queue.front();
//    totalDelta=0.0;
//    scrolls=0;
//    isSerifNow=true;
//    
//    
//    serifUpdateReady();
    
}

void SerifLayer::serifUpdateReady()
{
    //文字ラベル作成
    CCLabelTTF* label = CCLabelTTF::create(serifStr.c_str(), FONT_W6, 24, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);

    
    initLayout();
    
    label->setDimensions(CCSizeMake(serifSize.width, 0.0));
    label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height+label->getFontSize()*2.0));
    
    if ( label->getContentSize().height<serifSize.height ) {
        label->setDimensions(CCSizeMake(label->getContentSize().width, serifSize.height));
    }
    
    //スクロールビュー作成
    scroll = CCScrollView::create();
    scroll->setContainer(label);
    scroll->setViewSize(serifSize);
    scroll->setPosition(ccp(20.0,10.0));
    scroll->setDirection(kCCScrollViewDirectionVertical);
    serif->addChild(scroll);
    serifContentSize = scroll->getContentSize();
    
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(SerifLayer::serifUpdate));
    
    serif->runAction(CCSequence::create(CCEaseBackOut::create(CCScaleTo::create(0.2, 1.0)),func, NULL));
    
    //スクロール開始位置
    CCPoint startPoint = CCPoint(0.0, serifSize.height - serifContentSize.height);
    scroll->setContentOffset(startPoint);

}

void SerifLayer::serifUpdate(float delta)
{
    totalDelta+=delta;
    if ( totalDelta<1 ) {
        return;
    }
    scrolls++;

    float y = (serifSize.height + 0.5*scrolls) - serifContentSize.height;
    //    if ( serifSize.height/2.0 < y ) {
    if ( 0 < y ) {
        unschedule(schedule_selector(SerifLayer::serifUpdate));
        isSerifNow=false;
        return;
    }
    CCPoint startPoint = CCPoint(0.0, y);
    
    
    
    //    scroll->setContentOffsetInDuration(startPoint, 0.4);
    scroll->setContentOffset(startPoint);
}

