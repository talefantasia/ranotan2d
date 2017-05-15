//
//  TutorialLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/05/30.
//
//

#include "TutorialLayer.h"

TutorialLayer::TutorialLayer()
{
    
}

TutorialLayer::~TutorialLayer()
{
    
}

bool TutorialLayer::init(CCNode *node)
{
    if ( !CCLayer::init() ) {
        return false;
    }
    createNode(node);
    return true;
}

void TutorialLayer::createNode(cocos2d::CCNode *node)
{
    if ( node==NULL ) {
        return;
        
    }
    
    float x=0, y=0, z=0;
    CCLayer* layer = CCLayer::create();
    CCScale9Sprite* sprite = CCScale9Sprite::create("bitmap.png");
    sprite->setContentSize(node->getContentSize());
    sprite->setAnchorPoint(node->getAnchorPoint());
    sprite->setPosition(node->getPosition());
    layer->addChild(sprite);
    layer->getCamera()->getCenterXYZ(&x, &y, &z);
    layer->getCamera()->setCenterXYZ(x, y+0.0000003, z);
    this->addChild(layer);
    
    CCArray* ary = node->getChildren();
    CCLOG("Nodeqqq");
    if ( ary==NULL ) {
        return;
    }
    for (int i=0; i<ary->count()-1; i++) {
        CCNode* node2 = (CCNode*)ary->objectAtIndex(i);
        createNode(node2);
    }
    
}

TutorialLayer* TutorialLayer::create(CCNode* node)
{
    TutorialLayer* pRet = new TutorialLayer();
    if (pRet && pRet->init(node)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}
