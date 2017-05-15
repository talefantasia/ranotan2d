//
//  Ranotan2DAppDelegate.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/10.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "ItemViewScene.h"
#include "NotificationFactory.h"
#include "DateTimeUtil.h"
#include "SerifLayer.h"
#include "AsyncLabelNotice.h"

USING_NS_CC;
using namespace CocosDenshion;

#define winWidth 640
#define winHeightMin 960
double winHeight = winHeightMin;
double displayScale = 1;
int displayHorizontalMargin = 0;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
//    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
	int width = winWidth;
	winHeight = size.height * winWidth / size.width;
	displayScale = size.width / winWidth;
	if(winHeight < winHeightMin) {
		winHeight = winHeightMin;
		width = size.width * winHeightMin / size.height;
		displayScale = size.height / winHeightMin;
	}
	displayHorizontalMargin = (width - winWidth) / 2;
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(width, winHeight, kResolutionNoBorder);
	
    //2D設定
    CCDirector::sharedDirector()->setProjection(kCCDirectorProjection2D);
    
    // create a scene. it's an autorelease object
    CCScene *pScene = ItemViewScene::scene();

    // iOSのとき
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
    #endif

    
    // run
    pDirector->runWithScene(pScene);

    return true;
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCLayer *sceneLayer = (CCLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->lastObject();
    ItemViewScene* itemVIewScene = dynamic_cast<ItemViewScene*>(sceneLayer);
    if (itemVIewScene!=NULL) {
        CCLOG("Goto BackGrund");
    }

    NotificationFactory::setNotification();
    
    //バックグラウンド移動時の日付保存
    CCUserDefault::sharedUserDefault()->setStringForKey("LastDate", DateTimeUtil::getNowDateTime()->m_sString);
    
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    
    CCLayer *sceneLayer = (CCLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->lastObject();
    ItemViewScene* itemVIewScene = dynamic_cast<ItemViewScene*>(sceneLayer);
    if (itemVIewScene!=NULL) {
        CCLOG("Goto Foreground");
        itemVIewScene->createDialog(DialogLayer::DialogType_Comeback, itemVIewScene, callfunc_selector(ItemViewScene::addNoticeBookmark), NULL);
//        AsyncLabelNotice::create(NULL, NULL, NULL, NULL);
    }

    
}
