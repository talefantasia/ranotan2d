//
//  LabelNoticeLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/08/14.
//
//

#include "LabelNoticeLayer.h"

const ccColor4B ccc4Nomal = ccc4(0, 0, 0, 128);

LabelNoticeLayer::LabelNoticeLayer()
{
}

LabelNoticeLayer::~LabelNoticeLayer()
{
}

LabelNoticeLayer* LabelNoticeLayer::create()
{
    return create();
}

bool LabelNoticeLayer::init()
{
    
    ccColor4B color = ccc4Nomal;
    
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    setTouchEnabled(true);
    
    //レイアウトの初期設定
    initLayout();
    //コンテンツ配置
    
    return true;
}

void LabelNoticeLayer::initLayout()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //背景設置
    CCSprite* sprite = NULL;
    sprite = CCSprite::createWithSpriteFrameName("bg_common_dialog.png");
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    this->addChild(sprite, 0, TAG_DIALOG_BG);
}