//
//  ItemViewScene.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/10.
//
//

#include "ItemViewScene.h"
#include "CommonDefine.h"
#include "AnimationFactory.h"
#include "ViewItemFactory.h"
#include "ViewItemAsyncProcesser.h"
#include "CommonButton.h"
#include "DateTimeUtil.h"
#include "UserItemFactory.h"
#include "CreatorsDataFactory.h"
#include "ItemListTableLayer.h"
#include "BookMarkTableLayer.h"
#include "NotificationFactory.h"
#include "UserDataDefault.h"
#include "TextureFactory.h"
#include "SerifLayer.h"
#include "CalenderManager.h"
#include "BrowserLauncher.h"
#include "OpacityLayer.h"
#include "OpacitySprite.h"
#include "AsyncLabelNotice.h"
#include "SubCharactor.h"
#include "ReflectParticle.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

const CCSize ListTableSize = CCSizeMake(600, 500);
const CCSize ListCellSize = CCSizeMake(540.0, 182.0);
const CCSize ListImgSize = CCSizeMake(117.0, 145.0);
const CCSize ListBgImgSize = CCSizeMake(550, 800);
const CCSize DetailImgSize = CCSizeMake(450, 700);
const CCSize ItenNaviImgSize1 = CCSizeMake(140, 140);
const CCSize ItenNaviImgSize2 = CCSizeMake(84, 84);
const CCSize ItenNaviImgSize3 = CCSizeMake(56, 80);

const float detailImageDeg[3] = {-90.0, 0.0, 90.0};
const float baseDeg[13] = {-150,-120,-90.0,-60.0,-30.0,0.0,30.0,60.0,90.0,120,150};
const float detailOpacity[3] = {0, 255, 0};
CCPoint navicharaEffectPoint = ccp(40.0, 200.0);

struct THREADDATA{
    CCDictionary* dic;
    CCArray* list;
    int index;
};

static THREADDATA* threadData;

static pthread_mutex_t s_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_t s_thread;



ItemViewScene::ItemViewScene()
{
    listType = PLAN;
    viewType = LIST;
    planType = PLAN_ALL;
    exclusiveStatus=ExclusiveStatus_None;
    itemList = NULL;
    searchItemList=NULL;
    searchTitle=NULL;
    listIndex=0;
    ceparateIndex=0;
    isCamera=false;
    noticeItemList=NULL;
}

ItemViewScene::~ItemViewScene()
{
    listType = PLAN;
    viewType = LIST;
    planType = PLAN_ALL;
    exclusiveStatus=ExclusiveStatus_None;
    listIndex=0;
    ceparateIndex=0;
    CC_SAFE_RELEASE_NULL(itemList);
    CC_SAFE_RELEASE_NULL(searchItemList);
    CC_SAFE_RELEASE_NULL(searchTitle);
    CC_SAFE_RELEASE_NULL(noticeItemList);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_LAYER_CONTROLL_DETAIL_CAMERA);
    isCamera=false;

}

bool ItemViewScene::init()
{

    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    cache->addImage("img_detail_noimage_bg.png");
    
    if ( !CCLayerGradient::initWithColor(ccc4(51, 0, 204, 255), ccc4(0, 0, 0, 255), ccp(1, 1)) ) {
        return false;
    }
        
    std::string dir = CCFileUtils::sharedFileUtils()->getWritablePath() + "dir0/";
    
    mkdir(dir.c_str(), 0777);
    
    DIR* dp=opendir(dir.c_str());
    
    if (dp!=NULL)
    {
        struct dirent* dent;
        do{
            dent = readdir(dp);
            if (dent!=NULL) CCLog("Init Read Files %s", dent->d_name);
        }while(dent!=NULL);
        closedir(dp);
    }

    
    //Touch Event Enable
    setTouchEnabled(true);
    
    //設定系の読み込み
    initConfig();

    //背景
    initBackGround();
    
    //設定レイヤー追加
//    initSettingLayer();
    
    //Updateスケジューラ
    schedule(schedule_selector(ItemViewScene::update), 0.2);
    
    //キャラクター
    initCharactor();
    
    //台詞レイヤー
    initSerifLayer();
    
    doNoticeProcess();
    
    //データのチェックプロセス
    if (!doDataProcess()) return true;
    
    //表示プロセス
    doViewProcess();
    
    CCLOG("Setting Content is %s", UserDataDefault::sharedInstance()->getSettingContent() ? "Yes": "No" );
    
    createDialog(DialogLayer::DialogType_Welcome, NULL, NULL, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ItemViewScene::changeCamera), NOTIFICATION_LAYER_CONTROLL_DETAIL_CAMERA, NULL);

//    CCSprite* sprite = CCSprite::create("nc9228.png");
//    sprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2.0, 200));
////    sprite->setScale(0.8);
//    this->addChild(sprite, 100);

    return true;
}

void ItemViewScene::update(float delta)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int width = (int)winSize.width;
    int height = (int)winSize.height;
    int color = arc4random() % 4;
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("img_star_white_s.png");
//    if ( color==1 ) {
//        sprite = CCSprite::createWithSpriteFrameName("img_star_yellow_s.png");
//    }
//    if ( color==2 ) {
//        sprite = CCSprite::createWithSpriteFrameName("img_star_pink_s.png");
//    }
//    if ( color==3 ) {
//        sprite = CCSprite::createWithSpriteFrameName("img_star_green_s.png");
//    }
    int ranWidth = arc4random() % width;
    int ranHeight = arc4random() % height;
    float startScale = ((float)(arc4random() % 10)) / 10.0 + 1.0;
    sprite->setScale(startScale);
    sprite->setOpacity(0);
    sprite->setPosition(ccp( ranWidth, ranHeight ));
    CCActionInterval* fadeIn = CCFadeIn::create(0.5);
    CCActionInterval* fadeOut = CCFadeOut::create(1.5);
    CCCallFunc* func = CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent));
    sprite->runAction(CCSequence::create(fadeIn, fadeOut, func, NULL));
    addChild(sprite, zOrder_BackGround);
    
}

void ItemViewScene::initCharactor()
{
    CharactorLayer* layer = (CharactorLayer*)getChildByTag(TAG_ITEMVIEW_CHARACTOR);
    if ( layer!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_CHARACTOR);
    }
    layer = CharactorLayer::create();
    addChild(layer,zOrder_Charactor, TAG_ITEMVIEW_CHARACTOR);
    
}

void ItemViewScene::initSettingLayer()
{
    SettingLayer* layer = dynamic_cast<SettingLayer*>(getChildByTag(TAG_ITEMVIEW_SETTING));
    if ( layer!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_SETTING);
    }
    if ( viewType==LIST ) {
        layer =SettingLayer::create();
        layer->doFadeout();
        addChild(layer, zOrder_Setting, TAG_ITEMVIEW_SETTING);
    }
}

void ItemViewScene::initSerifLayer()
{
    SerifLayer* serifLayer = dynamic_cast<SerifLayer*>(getChildByTag(TAG_ITEMVIEW_CHARACTOR_SERIF));
    if ( serifLayer!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_CHARACTOR_SERIF, true);
    }
    serifLayer = SerifLayer::create();
    addChild(serifLayer, zOrder_Serif, TAG_ITEMVIEW_CHARACTOR_SERIF);
}

CharactorLayer* ItemViewScene::getCharactorLayer()
{
    CharactorLayer* layer = (CharactorLayer*)getChildByTag(TAG_ITEMVIEW_CHARACTOR);
    return layer;
}

void ItemViewScene::CharactorTap()
{
    CharactorLayer* layer = (CharactorLayer*)getChildByTag(TAG_ITEMVIEW_CHARACTOR);
    
    if ( layer==NULL ) {
        return;
    }
    
}

void ItemViewScene::initConfig()
{
    //TextureAtlas
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ViewItemScene.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("SubCharactor.plist");

}

void ItemViewScene::initBackGround()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint contentPoint = ccp(0.0, winSize.height/2.0+130);
    CCPoint infoPoint = ccp(winSize.width/2.0+420, winSize.height/2.0-130);
    CCPoint naviPoint = ccp(winSize.width/2.0, winSize.height/2.0-580);

    CCSprite* sprite = NULL;
    sprite = CCSprite::create("back.png");
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    addChild(sprite,zOrder_BackGround);

    
    //サークル左
    sprite = CCSprite::create("mahoujin_green.gif");
    sprite->setPosition(contentPoint);
    sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.0, 10.0)));
    addChild(sprite, zOrder_BackGround, TAG_ITEMVIEW_EFFECT_LEFT);
    
    //サークル右
    sprite = CCSprite::create("mahoujin_pink.gif");
    sprite->setPosition(infoPoint);
    sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.0, -10.0)));
    addChild(sprite, zOrder_BackGround, TAG_ITEMVIEW_EFFECT_RIGHT);

    //サークル右
    sprite = CCSprite::create("mahoujin_cyan.gif");
    sprite->setPosition(naviPoint);
    sprite->runAction(CCRepeatForever::create(CCRotateBy::create(1.0, -10.0)));
    addChild(sprite, zOrder_BackGround, TAG_ITEMVIEW_EFFECT_DOWN);
    
}

void ItemViewScene::doBackGroundEffect(int kind)
{
    CCSprite* parent = NULL;
    CCSprite* sprite = NULL;
    switch (kind) {
        case 0:
        {
            parent = (CCSprite*)getChildByTag(TAG_ITEMVIEW_EFFECT_LEFT);
            if ( parent==NULL ) return;
            sprite = CCSprite::createWithSpriteFrameName("bg_mahoujin_overray_1.png");
            sprite->setPosition(ccp(parent->getContentSize().width/2.0,parent->getContentSize().height/2.0) );
            parent->addChild(sprite);
            sprite->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on", CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent))));
            parent->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on-2"));
        }
            break;
        case 1:
        {
            parent = (CCSprite*)getChildByTag(TAG_ITEMVIEW_EFFECT_RIGHT);
            if ( parent==NULL ) return;
            sprite = CCSprite::createWithSpriteFrameName("bg_mahoujin_overray_1.png");
            sprite->setPosition(ccp(parent->getContentSize().width/2.0,parent->getContentSize().height/2.0) );
            parent->addChild(sprite);
            sprite->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on", CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent))));
            parent->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on-2"));
        }
            break;
        case 2:
        {
            parent = (CCSprite*)getChildByTag(TAG_ITEMVIEW_EFFECT_DOWN);
            if ( parent==NULL ) return;
            sprite = CCSprite::createWithSpriteFrameName("bg_mahoujin_overray_1.png");
            sprite->setPosition(ccp(parent->getContentSize().width/2.0,parent->getContentSize().height/2.0) );
            parent->addChild(sprite);
            sprite->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on", CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent))));
            parent->runAction(AnimationFactory::sharedInstance()->getActions("detail-item-on-2"));
        }
            
        default:
            break;
    }
}


CCScene* ItemViewScene::scene()
{
    CCScene* scene = CCScene::create();
    ItemViewScene* layer = new ItemViewScene();
    layer->init();
    layer->autorelease();
    scene->addChild(layer);
    return scene;
}

bool ItemViewScene::hasSearchItem()
{
    if ( getSearchItemList()==NULL ) {
        return false;
    }
    if ( getSearchItemList()->count()==0 ) {
        return false;
    }
    return true;
}
bool ItemViewScene::doDataProcess()
{
    
    //サーバ同期レイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_SERVERSYNC)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_SERVERSYNC);
    }

    itemList = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    //データが無い場合は取得プロセスへ
    if ( itemList==NULL ) {
        addChild(DialogLayer::create(DialogLayer::DialogType_FirstUpdateConfirm, this, callfunc_selector(ItemViewScene::doServerSyncProcess), NULL), zOrder_Dialog);
        return false;
    }
    if ( itemList->count()==0 ) {
        addChild(DialogLayer::create(DialogLayer::DialogType_FirstUpdateConfirm, this, callfunc_selector(ItemViewScene::doServerSyncProcess), NULL), zOrder_Dialog);
        return false;        
    }
    if ( planType==PLAN_BOOKMARK ) {
        
    }
    mathCeparateIndex();
    
    listIndex=ceparateIndex;
    return true;
}

void ItemViewScene::doViewProcess()
{
    switch (viewType) {
        case LIST:
            doListProcess(false);
            break;
        case DETAIL:
            doDetailProcess();
            break;
            
        default:
            break;
    }
}

void ItemViewScene::doListProcess(bool doSync)
{
    //リストレイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_LIST)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_LIST);
    }
    //詳細レイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_DETAIL)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_DETAIL);
    }
    //サーバ同期レイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_SERVERSYNC)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_SERVERSYNC);
    }
    //Particleがある場合は削除
    removeParticle();
    //プロセスチェック
    //手動同期の場合は問答無用で取得プロセスへ
    if ( doSync ) {
        doServerSyncProcess();
        return;
    }
    
//    initSettingLayer();
    
    //それ以外の場合は普通に表示
    createList();
}

void ItemViewScene::mathCeparateIndex()
{
    itemList=ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    if ( planType==PLAN_BOOKMARK ) {
        itemList=CreatorsDataFactory::sharedInstance()->getCheckList();
    }
    ceparateIndex=0;
    for (int i=0; i<itemList->count(); i++) {
        ViewItem* item = (ViewItem*)itemList->objectAtIndex(i);
        if ( !DateTimeUtil::isAfter(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
            ceparateIndex=i;
            break;
        }
    }
}

void ItemViewScene::doServerSyncProcess()
{
    removeParticle();
    doParticle();
    addChild(ServerSyncLayer::create(this, callfunc_selector(ItemViewScene::serverSyncCallback)),zOrder_ItemList,TAG_ITEMVIEW_SERVERSYNC);
}

void ItemViewScene::serverSyncCallback()
{
    doNoticeProcess();
    if (!doDataProcess()) return;
    doViewProcess();
    createDialog(DialogLayer::DialogType_Welcome, NULL, NULL, NULL);
}

void ItemViewScene::createList()
{
    addChild(ItemListLayer::create(listIndex, listType, planType), zOrder_ItemList,TAG_ITEMVIEW_LIST);
}

void ItemViewScene::doDetailProcess()
{
    //リストレイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_LIST)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_LIST);
    }
    //詳細レイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_DETAIL)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_DETAIL);
    }
    //サーバ同期レイヤがある場合は破棄
    if ( getChildByTag(TAG_ITEMVIEW_SERVERSYNC)!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_SERVERSYNC);
    }
    //Particleがある場合は削除
    removeParticle();
    doParticle();
    initSettingLayer();
    
    addChild(ItemDetailLayer::create(listIndex, listType, planType), zOrder_ItemDetail,TAG_ITEMVIEW_DETAIL);
    
}

void ItemViewScene::doNoticeProcess()
{
    
    if ( noticeItemList==NULL || noticeItemList->count()==0 ) {
        return;
    }
    
    CCString* serif = CCString::create("☆★☆新作情報があります☆★☆\n");
    for (int i=0; i<noticeItemList->count(); i++) {
        ViewItem* item = (ViewItem*)noticeItemList->objectAtIndex(i);
        CCString* no = CCString::createWithFormat("%d. ", i+1);
        serif->m_sString.append(no->m_sString);
        serif->m_sString.append(item->getTitle()->m_sString);
        serif->m_sString.append("\n");
    }
    
    SerifLayer* serifLayer = SerifLayer::create();
    serifLayer->setTextString(serif->getCString());
    serifLayer->setScrollStartDelay(3.0);
    addChild(serifLayer, zOrder_Dialog, TAG_DETAIL_SERIF);
 
    noticeItemList=NULL;
}


void ItemViewScene::createDialog(DialogLayer::DialogType type,  cocos2d::CCObject* target, cocos2d::SEL_CallFunc doneSelector, CCDictionary* params)
{
    DialogLayer* dialog = dynamic_cast<DialogLayer*>( getChildByTag(TAG_ITEMVIEW_DIALOG) );
    if ( dialog!=NULL ) {
        removeChildByTag(TAG_ITEMVIEW_DIALOG, true);
    }
    dialog = DialogLayer::create(type, target, doneSelector, params);
    addChild(dialog, zOrder_Dialog, TAG_ITEMVIEW_DIALOG);
}

void ItemViewScene::changeViewSearch()
{
    setViewType(LIST);
    setListType(SEARCH);
    doDataProcess();
    doViewProcess();
    createDialog(DialogLayer::DialogType_ChangeListType
                        , NULL, NULL, NULL);
}

void ItemViewScene::createSerif(const char *serif, bool isDuplicate)
{
    
    SerifLayer* serifLayer = dynamic_cast<SerifLayer*>(getChildByTag(TAG_ITEMVIEW_CHARACTOR_SERIF));
    if ( serifLayer!=NULL && !isDuplicate) {
        removeChildByTag(TAG_ITEMVIEW_CHARACTOR_SERIF, true);
    }
    
    serifLayer = SerifLayer::create();
    serifLayer->setTextString(serif);
}

void ItemViewScene::createSerif(const char *serif, cocos2d::CCPoint basePos, bool isDuplicate)
{
    SerifLayer* serifLayer = dynamic_cast<SerifLayer*>(getChildByTag(TAG_ITEMVIEW_CHARACTOR_SERIF));
    if ( serifLayer==NULL) {
        return;
    }
    
    serifLayer = SerifLayer::create();
    serifLayer->setBasePos(basePos);
    serifLayer->setTextString(serif);

}

void ItemViewScene::addNoticeBookmark()
{
    std::string notice = NotificationFactory::createNotificationItemInApp();
    if ( notice.length()!=0 ){
        SerifLayer* serifLayer = SerifLayer::create();
        serifLayer->setTextString(notice.c_str());
        CCDirector::sharedDirector()->getRunningScene()->addChild(serifLayer);
    }
}

void ItemViewScene::changeCamera()
{
    if ( viewType==DETAIL ) {
        float delta = 0.00000015;
        if ( isCamera ) {
            delta=-0.00000015;
            isCamera=false;
        } else {
            isCamera=true;
        }
        CCLOG("changeCamera");
        ItemDetailLayer* layer = (ItemDetailLayer*)getChildByTag(TAG_ITEMVIEW_DETAIL);
        float x=0, y=0, z=0;
        layer->getCamera()->getCenterXYZ(&x, &y, &z);
        layer->getCamera()->setCenterXYZ(x, y, z+delta);
    }
}

void ItemViewScene::restartViewItemScene()
{
    listType = PLAN;
    viewType = LIST;
    exclusiveStatus=ExclusiveStatus_None;
    itemList = NULL;
    searchItemList=NULL;
    searchTitle=NULL;
    listIndex=0;
    ceparateIndex=0;
    isCamera=false;
    init();

}

bool ItemViewScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if ( viewType!=LIST ) {
        return false;
    }
    CCPoint point = pTouch->getLocation();
    if ( point.x < 20 ) {
        touchStart = point;
        return true;
    }
    return false;
}

void ItemViewScene::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //SettingLayer Move
    SettingLayer* layer = (SettingLayer*)getChildByTag(TAG_ITEMVIEW_SETTING);
    if ( layer==NULL ) {
        return;
    }
    if ( layer->getClose()==false ) {
        return;
    }
    CCPoint point = pTouch->getLocation();
    CCPoint layerPoint = layer->getPosition();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float swipeX = (point.x-touchStart.x);
    float posX = swipeX - winSize.width;
    posX = (posX>0.0) ? 0.0: posX;
    layer->setPosition(ccp(posX,0.0));
    if ( swipeX > 150.0 ) {
        layer->setClose(false);
        layer->setTouchEnabled(true);
        CCEaseIn* act = CCEaseIn::create(CCMoveTo::create(0.2, ccp(0.0,0.0)),2.0);
        CCCallFunc* func = CCCallFunc::create(layer, callfunc_selector(SettingLayer::initLayout));
        CCActionInterval* act1 = CCSequence::create(act, func, NULL);
        layer->runAction(act1);
    }
    
}

void ItemViewScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    SettingLayer* layer = (SettingLayer*)getChildByTag(TAG_ITEMVIEW_SETTING);
    if ( layer==NULL ) {
        return;
    }
    if ( layer->getClose()==false ) {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint point = pTouch->getLocation();
    float swipeX = (point.x-touchStart.x);

    if ( swipeX<=150.0 ) {
        CCEaseBackOut* act = CCEaseBackOut::create(CCMoveTo::create(0.2, ccp(-winSize.width,0.0)));
        layer->runAction(act);
    }
}

void ItemViewScene::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

}

void ItemViewScene::doParticle()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    Reflectparticle* particle = Reflectparticle::create(ccp(winSize.width * 10, winSize.height*10));
    addChild(particle,zOrder_particle, TAG_ITEMVIEW_PARTICLE);
    
    CCMoveTo* move1 = CCMoveTo::create(0.00, ccp(winSize.width/2.0, winSize.height));
    
    CCMoveTo* move2 = CCMoveTo::create(0.2, ccp(0.0, winSize.height));
    CCMoveTo* move3 = CCMoveTo::create(0.6, ccp(0.0, 0.0));
    CCMoveTo* move4 = CCMoveTo::create(0.2, ccp(winSize.width/2.0, 0.0));
    
    CCCallFunc* func = CCCallFunc::create(particle, callfunc_selector(Reflectparticle::doMove));

    particle->runAction(CCSequence::create(CCDelayTime::create(0.5), move1,move2,move3,move4,CCDelayTime::create(1.5),func, NULL));
    
    
    
    particle = Reflectparticle::create(ccp(winSize.width * 10, winSize.height*10));
    addChild(particle,zOrder_particle, TAG_ITEMVIEW_PARTICLE);
    
    move1 = CCMoveTo::create(0.00, ccp(winSize.width/2.0, 0.0));
    
    move2 = CCMoveTo::create(0.2, ccp(winSize.width, 0.0));
    move3 = CCMoveTo::create(0.6, ccp(winSize.width, winSize.height));
    move4 = CCMoveTo::create(0.2, ccp(winSize.width/2.0, winSize.height));
    
    func = CCCallFunc::create(particle, callfunc_selector(Reflectparticle::doMove));
    particle->runAction(CCSequence::create(CCDelayTime::create(0.5), move1,move2,move3,move4,CCDelayTime::create(1.5),func, NULL));
}

void ItemViewScene::removeParticle()
{
    CCArray* ary = getChildren();
    if ( ary==NULL || ary->count()==0 ) {
        return;
    }
    CCArray* removeAry = CCArray::create();
    for (int i=0; i<ary->count(); i++) {
        CCNode* node = (CCNode*)ary->objectAtIndex(i);
        if ( node==NULL ) {
            continue;
        }
        if ( node->getTag()==TAG_ITEMVIEW_PARTICLE ) {
            removeAry->addObject(node);
        }
    }
    if ( removeAry->count()==0 ) return;
    for (int i=0; i<removeAry->count(); i++) {
        CCNode* node = (CCNode*)removeAry->objectAtIndex(i);
        node->removeFromParentAndCleanup(true);
    }
}

/*******************************************************/
/*******************************************************/
/**********           ItemListLayer         ************/
/*******************************************************/
/*******************************************************/
ItemListLayer::ItemListLayer()
{
    itemList=NULL;
    noticeList=NULL;
    table=NULL;
    listType=PLAN;
    planType=PLAN_ALL;
    forcsItem=-1;
    itemCnt=0;
    beforeOffsetY=0.0;
    scrollType=INCREMENT;
    bookmarkType=ALL;
    circleDetail = 500.0;
}

ItemListLayer::~ItemListLayer()
{
    CC_SAFE_RELEASE(itemList);
    CC_SAFE_RELEASE(noticeList);
    CC_SAFE_RELEASE(table);
    listType=PLAN;
    planType=PLAN_ALL;
    forcsItem=-1;
    itemCnt=0;
    beforeOffsetY=0.0;
    scrollType=INCREMENT;
    bookmarkType=ALL;
    circleDetail = 500.0;
    CCLOG("%s","~ItemListLayer");
}

bool ItemListLayer::init(int index, ListType type, PlanType planType)
{
//    if ( !CCLayerGradient::initWithColor(ccc4(51, 0, 204, 255), ccc4(0, 0, 0, 255), ccp(1, 1)) ) {
    if ( !CCLayer::init() ) {
        return false;
    }
    
    
    setListType(type);
    setPlanType(planType);
    
    initData();
    initNoticeData();
    initLayout(index);
    createList(index);

    TextureFactory::shardInstance()->initTextures(itemList, 5);
    
    
    return true;
}

void ItemListLayer::onEnter()
{
    CCLOG("Last Update is %s", UserDataDefault::sharedInstance()->getUpdateDate()->getCString());
    CCLOG("Last Update Confirm is %s", UserDataDefault::sharedInstance()->getUpdateConfirmDate()->getCString());
    CCLayer::onEnter();
    if ( UserDataDefault::sharedInstance()->isUpdate() && UserDataDefault::sharedInstance()->isUpdateConfirm() ) {
        CCNode* node = CCNode::create();
        CCDelayTime* delay = CCDelayTime::create(2.0);
        CCCallFunc* func1 = CCCallFunc::create(this, callfunc_selector(ItemListLayer::confirmSyncFromTimelimit));
        CCCallFunc* func2 = CCCallFunc::create(node, callfunc_selector(CCNode::removeFromParent));
        node->runAction(CCSequence::create(delay, func1, func2, NULL));
        this->addChild(node);
    }
}

ItemListLayer* ItemListLayer::create()
{
    ItemListLayer* pRet = new ItemListLayer();
    if (pRet && pRet->init(0, PLAN, PLAN_ALL)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

ItemListLayer* ItemListLayer::create(int index, ListType listType, PlanType planType)
{
    ItemListLayer* pRet = new ItemListLayer();
    if (pRet && pRet->init(index, listType, planType)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void ItemListLayer::initData()
{
    if ( listType==PLAN ) {
        setItemList(ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate());
        if ( planType==PLAN_BOOKMARK ) {
            setItemList(CreatorsDataFactory::sharedInstance()->getCheckList());
        }
    } else if ( listType==SEARCH ) {
        CCArray* ary = (ViewItemFactory::sharedInstance()->createSearchList());
        setItemList(ary);
    } else {
        CreatorsDataFactory::sharedInstance()->init();
        setItemList(UserItemFactory::sharedInstance()->getUserItemList());
    }
}

void ItemListLayer::initNoticeData()
{
}

void ItemListLayer::initLayout(int index)
{
    removeAllChildrenWithCleanup(true);
    if ( listType==PLAN ) {
        initLayoutPlan();
    } else if ( listType==SEARCH ) {
        initLayoutSearch();
    } else {
        initLayoutBookMark(index);
    }

}

void ItemListLayer::initLayoutPlan()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* sprite = NULL;
    CCLabelTTF* label = NULL;
    CCScale9Sprite* scaleSprite = NULL;
    
    //タイトル
   scaleSprite = CCScale9Sprite::create("nc9228.png");
    CCSize size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 +470.0- size.height/2.0));
//    scaleSprite->setContentSize(CCSizeMake(winSize.width+20.0, size.height+40.0));
    addChild(scaleSprite, zOrder_Title, TAG_LIST_TITLE);
    CCString* str = CCString::createWithFormat("発売予定表（全%d件）",itemList->count());
    if ( planType==PLAN_BOOKMARK ) {
        int planCnt = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate()->count();
        str = CCString::createWithFormat("発売予定表（全%d件中%d件）", planCnt, itemList->count());
    }
    
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 32, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, size.height/2.0+12.0));
    label->setColor(ccc3(255, 255, 255));
    scaleSprite->addChild(label);

    str = CCString::createWithFormat("最終更新日 %s",UserDataDefault::sharedInstance()->getUpdateDate()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 20, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0-25, size.height/2.0-45.0));
    label->setColor(ccc3(255, 255, 00));
    scaleSprite->addChild(label);
    
    label = CCLabelTTF::create("※データは発売後１５日間保持されます", FONT_W3, 20, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, size.height/2.0-18));
    label->setColor(ccc3(224, 224, 224));
    scaleSprite->addChild(label);
    
    //LeftMenu
    planMenu(0);
    
    //menu
    //フッダーメニュー
    scaleSprite = CCScale9Sprite::createWithSpriteFrameName("bg_common_menu_v2.png");
    size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 - 450.0 + size.height/2.0));
    addChild(scaleSprite, zOrder_ItemMenu, TAG_LIST_MENU);
    
    CommonButton* button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_serversync_v2.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::confirmSync));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*11.0/12.0, size.height/2.0));
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_bookmark_v2.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkReady));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*9.0/12.0, size.height/2.0));

//    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_label_info.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkReady));
//    scaleSprite->addChild(button);
//    button->setPosition(ccp(size.width*7.0/12.0, size.height/2.0));

    
    CCArray* searchs = ViewItemFactory::sharedInstance()->createSearchList();
    if (searchs!=NULL &&searchs->count()>0) {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_search_result.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeSearch));
        scaleSprite->addChild(button);
        button->setPosition(ccp(size.width*5.0/12.0, size.height/2.0));
    }
        
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_credit.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openCredit));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*1.0/12.0, size.height/2.0));

    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_help.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openHelp));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*3.0/12.0, size.height/2.0));
    
//    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_label_info.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openHelp));
//    scaleSprite->addChild(button);
//    button->setPosition(ccp(size.width*5.0/12.0, size.height/2.0));

    //
//    naviCharaMarisaPos =ccp(540, winSize.height/2.0+200);
//    sprite = CCSprite::create("illust_common_marisa_270.png");
//    sprite->setPosition(naviCharaMarisaPos);
//    CCActionInterval* moveDown = CCMoveBy::create(0.5, ccp(0, -20));
//    CCActionInterval* moveUp = CCMoveBy::create(1.0, ccp(0, 20));
//    CCSequence* seq = CCSequence::create(moveDown, moveUp, NULL);
//    sprite->runAction(CCRepeatForever::create(seq));
//    
//    addChild(sprite, zOrder_Title, TAG_LIST_NAVICHARA);

}

void ItemListLayer::initLayoutSearch()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* sprite = NULL;
    CCLabelTTF* label = NULL;
    CCScale9Sprite* scaleSprite = NULL;
    
    //タイトル
    scaleSprite = CCScale9Sprite::create("nc9228.png");
    CCSize size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 +470.0- size.height/2.0));
//    scaleSprite->setContentSize(CCSizeMake(winSize.width+20.0, size.height+40.0));
    addChild(scaleSprite, zOrder_Title, TAG_LIST_TITLE);
    CCString* str = CCString::createWithFormat("検索結果（全%d件）",itemList->count());
    
    label = CCLabelTTF::create(str->getCString(), FONT_W3, 32, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, size.height/2.0+10.0));
    label->setColor(ccc3(255, 255, 255));
    scaleSprite->addChild(label);
    
    searchMenu(0);
    
    //menu
    //フッダーメニュー
    scaleSprite = CCScale9Sprite::createWithSpriteFrameName("bg_common_menu_v2.png");
    size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 - 450.0 + size.height/2.0));
    addChild(scaleSprite, zOrder_ItemMenu, TAG_LIST_MENU);
    
    CommonButton* button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_plan.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changePlanReady));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*11.0/12.0, size.height/2.0));
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_bookmark_v2.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkReady));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*9.0/12.0, size.height/2.0));
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_credit.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openCredit));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*1.0/12.0, size.height/2.0));
    ItemViewScene* scene = dynamic_cast<ItemViewScene*>(getParent());

}


void ItemListLayer::tapCharactor()
{
    ItemViewScene* scene = dynamic_cast<ItemViewScene*>(getParent());
    if ( scene==NULL ) {
        return;
    }
    scene->CharactorTap();
    
}

void ItemListLayer::initLayoutBookMark(int index)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* sprite = NULL;
    CCLabelTTF* label = NULL;
    CommonButton* button = NULL;


    //タイトル
    int frameZorder = zOrder_Title+1;
    CCScale9Sprite* scaleSprite = CCScale9Sprite::create("label_bookmark_title.png");
    if ( bookmarkType==AUTHOR ) {
        frameZorder= zOrder_Title+1;
        scaleSprite = CCScale9Sprite::create("label_bookmark_title_author.png");
    } else if ( bookmarkType==CREATOR ) {
        frameZorder= zOrder_Title+1;
        scaleSprite = CCScale9Sprite::create("label_bookmark_title_creator.png");
    }

    //フレーム設置
    sprite = CCSprite::create("frameback.png");
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+10));
    addChild(sprite, frameZorder);

    
    CCSize size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 +470.0- size.height/2.0));
    addChild(scaleSprite, zOrder_Title, TAG_LIST_TITLE);
    
    CCString* str = CCString::createWithFormat("ブックマーク（全%d件）",itemList->count());
    if ( bookmarkType==AUTHOR ) {
        str = CCString::createWithFormat("ブックマーク：作家");
    } else if (bookmarkType==CREATOR) {
        str = CCString::createWithFormat("ブックマーク：イラスト");
    }
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 32, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, size.height/2.0));
    label->setColor(ccc3(255, 255, 255));
    scaleSprite->addChild(label);
    
    int possibleCnt = MAX_COUNT_USER_ITEM - itemList->count();
    possibleCnt = (possibleCnt<0) ? 0: possibleCnt;
    str = CCString::createWithFormat("登録可能な件数：あと%d件", possibleCnt);
    if ( possibleCnt==0 ) {
        str = CCString::createWithFormat("登録件数がMAXです。(上限：%d件)", MAX_COUNT_USER_ITEM);
    }
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 20, CCSizeMake(winSize.width-150.0, 50), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0-25, size.height/2.0-45.0));
    label->setColor(ccc3(255, 255, 00));
    scaleSprite->addChild(label);
    

    //LeftMenu
    bookmarkMenu(index);
    
    //menu
    //フッダーメニュー
    if ( bookmarkType==ALL ) {
        scaleSprite = CCScale9Sprite::createWithSpriteFrameName("bg_common_menu_v2.png");
    } else if (bookmarkType==AUTHOR) {
        scaleSprite = CCScale9Sprite::createWithSpriteFrameName("bg_common_menu_author.png");
    } else if ( bookmarkType==CREATOR ) {
        scaleSprite = CCScale9Sprite::createWithSpriteFrameName("bg_common_menu_creator.png");        
    }
    size = scaleSprite->getContentSize();
    scaleSprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0 - 450.0 + size.height/2.0));
    addChild(scaleSprite, zOrder_ItemMenu, TAG_LIST_MENU);
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_plan.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changePlanReady));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*10.0/12.0, size.height/2.0));

    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_credit.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openCredit));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*2.0/12.0, size.height/2.0));

    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_help.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::openHelp));
    scaleSprite->addChild(button);
    button->setPosition(ccp(size.width*4.0/12.0, size.height/2.0));
    
}

void ItemListLayer::bookmarkMenu(int index)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* sprite = NULL;
    CommonButton* button = NULL;
    
    if (getChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU)!=NULL) {
        removeChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU, true);
    }
    
    if ( getChildByTag(TAG_LIST_BOOKMARK_OVERRAY)!=NULL ) {
        removeChildByTag(TAG_LIST_BOOKMARK_OVERRAY, true);
    }
    
    //LeftMenu
    if ( bookmarkType==ALL ) {
        sprite = CCSprite::createWithSpriteFrameName("bg_bookmark_left_menu.png");
        addChild(ItemListTableLayer::create(index, BOOKMARK, planType), 0, TAG_LIST_BOOKMARK_OVERRAY);
    } else if ( bookmarkType==AUTHOR ) {
        sprite = CCSprite::createWithSpriteFrameName("bg_bookmark_left_menu_author.png");
        
        addChild(BookMarkTableLayer::create(AUTHOR), zOrder_BookmarkOverray, TAG_LIST_BOOKMARK_OVERRAY);
        
    } else {
        sprite = CCSprite::createWithSpriteFrameName("bg_bookmark_left_menu_illust.png");
        addChild(BookMarkTableLayer::create(CREATOR), zOrder_BookmarkOverray, TAG_LIST_BOOKMARK_OVERRAY);
    }
    
    sprite->setPosition(ccp(winSize.width/2.0-240, winSize.height/2.0-10));
    addChild(sprite, zOrder_ItemMenu, TAG_LIST_BOOKMARK_LEFT_MENU);
    CCSize leftmenuSize = sprite->getContentSize();
    
    if ( bookmarkType==ALL ) {
        CCSprite* btn = CCSprite::createWithSpriteFrameName("btn_bookmark_all_on.png");
        btn->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 + 130));
        sprite->addChild(btn);
    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_bookmark_all_off.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkTypeAll));
        button->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 + 130));
        sprite->addChild(button);
    }
    
    if ( bookmarkType==AUTHOR ) {
        CCSprite* btn = CCSprite::createWithSpriteFrameName("btn_bookmark_author_on.png");
        btn->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0));
        sprite->addChild(btn);
    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_bookmark_author_off.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkTypeAuthor));
        button->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0));
        sprite->addChild(button);
    }
    
    if ( bookmarkType==CREATOR ) {
        CCSprite* btn = CCSprite::createWithSpriteFrameName("btn_bookmark_illust_on.png");
        btn->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 - 130));
        sprite->addChild(btn);
    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_bookmark_illust_off.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changeBookmarkTypeCreator));
        button->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 - 130));
        sprite->addChild(button);
    }
}

void ItemListLayer::planMenu(int index)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCSprite* sprite = NULL;
    CommonButton* button = NULL;
    
    if (getChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU)!=NULL) {
        removeChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU, true);
    }
    
    if ( getChildByTag(TAG_LIST_BOOKMARK_OVERRAY)!=NULL ) {
        removeChildByTag(TAG_LIST_BOOKMARK_OVERRAY, true);
    }
    
    //LeftMenu
    sprite = CCSprite::createWithSpriteFrameName("bg_bookmark_left_menu.png");
    
    sprite->setPosition(ccp(winSize.width/2.0-240, winSize.height/2.0-10));
    addChild(sprite, zOrder_ItemMenu, TAG_LIST_BOOKMARK_LEFT_MENU);
    CCSize leftmenuSize = sprite->getContentSize();
    
    if ( planType==PLAN_ALL ) {
        CCSprite* btn = CCSprite::createWithSpriteFrameName("btn_bookmark_all_on.png");
        btn->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 + 65));
        sprite->addChild(btn);
    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_bookmark_all_off.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changePlanTypeAll));
        button->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 + 65));
        sprite->addChild(button);
    }
    
    if ( planType==PLAN_BOOKMARK ) {
        CCSprite* btn = CCSprite::createWithSpriteFrameName("btn_list_mark_on.png");
        btn->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0 - 65));
        sprite->addChild(btn);
    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_list_mark_off.png"), CCSizeZero, this, callfuncO_selector(ItemListLayer::changePlanTypeBookmark));
        button->setPosition(ccp(leftmenuSize.width/2.0, leftmenuSize.height/2.0-65));
        sprite->addChild(button);
    }
    
}

void ItemListLayer::searchMenu(int index)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* sprite = NULL;
    CommonButton* button = NULL;
    
    if (getChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU)!=NULL) {
        removeChildByTag(TAG_LIST_BOOKMARK_LEFT_MENU, true);
    }
    
    if ( getChildByTag(TAG_LIST_BOOKMARK_OVERRAY)!=NULL ) {
        removeChildByTag(TAG_LIST_BOOKMARK_OVERRAY, true);
    }
    
    //LeftMenu
    sprite = CCSprite::createWithSpriteFrameName("bg_bookmark_left_menu.png");
    
    sprite->setPosition(ccp(winSize.width/2.0-240, winSize.height/2.0-10));
    addChild(sprite, zOrder_ItemMenu, TAG_LIST_BOOKMARK_LEFT_MENU);
}

void ItemListLayer::createList(int index)
{
    
    if ( getChildByTag(TAG_LIST_TABLELAYER)!=NULL ) {
        removeChildByTag(TAG_LIST_TABLELAYER, true);
    }
    
    if ( listType==PLAN ) {
        addChild(ItemListTableLayer::create(index, PLAN, planType), 0, TAG_LIST_TABLELAYER);
    } else if (listType==SEARCH) {
        addChild(ItemListTableLayer::create(0, SEARCH, planType), 0, TAG_LIST_TABLELAYER);
    } else if (listType==BOOKMARK && bookmarkType==ALL) {
 
    }
    
    
}

void ItemListLayer::confirmSync()
{
    ((ItemViewScene*)getParent())->createDialog(DialogLayer::DialogType_UpdateConfirm, this, callfunc_selector(ItemListLayer::doServerSync), NULL);
}

void ItemListLayer::confirmSyncFromTimelimit()
{
    UserDataDefault::sharedInstance()->modifyUpdateConfirmDate();
    ((ItemViewScene*)getParent())->createDialog(DialogLayer::DialogType_UpdateConfirmTimeLimit, this, callfunc_selector(ItemListLayer::doServerSync), NULL);
}


void ItemListLayer::doServerSync()
{
    ((ItemViewScene*)getParent())->setListIndex(0);
    ((ItemViewScene*)getParent())->doListProcess(true);
}

void ItemListLayer::openCredit()
{
    CCDictionary* dic = CCDictionary::create();
    if ( listType==BOOKMARK && bookmarkType==AUTHOR ) {
        dic->setObject(CCInteger::create(1), "BgType");
    } else if ( listType==BOOKMARK && bookmarkType==CREATOR ) {
        dic->setObject(CCInteger::create(2), "BgType");
    }
    
    ((ItemViewScene*)getParent())->createDialog(DialogLayer::DialogType_Credit, NULL, NULL, dic);
}

void ItemListLayer::openHelp()
{
    CCDictionary* dic = CCDictionary::create();
    if ( listType==BOOKMARK && bookmarkType==AUTHOR ) {
        dic->setObject(CCInteger::create(1), "BgType");
    } else if ( listType==BOOKMARK && bookmarkType==CREATOR ) {
        dic->setObject(CCInteger::create(2), "BgType");
    }

    ((ItemViewScene*)getParent())->createDialog(DialogLayer::DialogType_Help, NULL, NULL, dic);
    
}


void ItemListLayer::changeDetailReady(int listIndex)
{
    changeDetailReady(listIndex, listType);
    
}

void ItemListLayer::changeDetailReady(int listIndex, ListType type)
{
    changeDetailReady(listIndex, type, planType);
}

void ItemListLayer::changeDetailReady(int listIndex, ListType type, PlanType plan)
{
    forcsItem=listIndex;
    planType=plan;
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE2, MagicCircleEffectStatus::StarMoveType_OUT);
    }
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Color_White_fadeout, NULL, NULL, NULL);
    listType=type;
    runAction(CCSequence::create(CCDelayTime::create(0.02), CCCallFunc::create(this, callfunc_selector(ItemListLayer::changeDetail)), NULL));

    //    CCSprite* naviChara = (CCSprite*)getChildByTag(TAG_LIST_NAVICHARA);
    //    CCActionInterval* scale2 = CCScaleTo::create(0.3, 0.0, 1.0);
    //    CCActionInterval* delay = CCDelayTime::create(0.2);
    //    CCActionInterval* move = CCMoveBy::create(0.1, ccp(0.0, 500));
    //    CCSequence* seq1 = CCSequence::create(delay, move, NULL);
    //    CCSpawn* spawn = CCSpawn::create(scale2, seq1, NULL);
    //    naviChara->runAction(CCSequence::create(spawn, CCCallFunc::create(this, callfunc_selector(ItemListLayer::changeDetail)), NULL));
    
}


void ItemListLayer::changeDetail()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->setListIndex(forcsItem);
    scene->setListType(listType);
    scene->setPlanType(planType);
    scene->setViewType(DETAIL);
    scene->doViewProcess();
}

void ItemListLayer::changeBookmarkReady()
{
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE, MagicCircleEffectStatus::StarMoveType_OUT);
    }
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Color_White_fadeinout
                        , NULL, NULL, NULL);
    runAction(CCSequence::create(CCDelayTime::create(0.05), CCCallFunc::create(this, callfunc_selector(ItemListLayer::changeBookmark)), NULL));
}


void ItemListLayer::changeBookmark()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->setListIndex(0);
    scene->setViewType(LIST);
    scene->setListType(BOOKMARK);
    scene->doViewProcess();
    scene->createDialog(DialogLayer::DialogType_ChangeListType
                        , NULL, NULL, NULL);
}

void ItemListLayer::changePlanReady()
{
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE, MagicCircleEffectStatus::StarMoveType_OUT);
    }
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Color_White_fadeinout
                        , NULL, NULL, NULL);
    runAction(CCSequence::create(CCDelayTime::create(0.05), CCCallFunc::create(this, callfunc_selector(ItemListLayer::changePlan)), NULL));
}

void ItemListLayer::changePlan()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->setViewType(LIST);
    scene->setListType(PLAN);
    scene->doDataProcess();
    scene->doViewProcess();
    scene->createDialog(DialogLayer::DialogType_ChangeListType
                        , NULL, NULL, NULL);
}

void ItemListLayer::changeSearch()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->changeViewSearch();
}

void ItemListLayer::changeBookmarkTypeReady( BookmarkType type )
{
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE,MagicCircleEffectStatus::StarMoveType_OUT);
    }
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Color_White_fadeinout
                        , NULL, NULL, NULL);
    bookmarkType=type;
    runAction(CCSequence::create(CCDelayTime::create(0.05), CCCallFunc::create(this, callfunc_selector(ItemListLayer::changeBookmarkType)), NULL));
}


void ItemListLayer::changeBookmarkType()
{
    bookmarkMenu(0);
    initLayout(0);
}

void ItemListLayer::changePlanTypeReady( PlanType type )
{
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE, MagicCircleEffectStatus::StarMoveType_OUT);
    }
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Color_White_fadeinout
                        , NULL, NULL, NULL);
    planType=type;
    runAction(CCSequence::create(CCDelayTime::create(0.05), CCCallFunc::create(this, callfunc_selector(ItemListLayer::changePlanType)), NULL));
}

void ItemListLayer::changePlanType()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->setListIndex(0);
    scene->setViewType(LIST);
    scene->setListType(PLAN);
    scene->setPlanType(planType);
    scene->doDataProcess();
    scene->doViewProcess();
    scene->createDialog(DialogLayer::DialogType_ChangeListType
                        , NULL, NULL, NULL);
}


/*******************************************************/
/*******************************************************/
/**********                                 ************/
/**********          ItemDetailLayer        ************/
/**********                                 ************/
/*******************************************************/
/*******************************************************/

ItemDetailLayer::ItemDetailLayer()
{
    itemList=NULL;
    listIndex=0;
    isNaviChange=true;
    listType=PLAN;
    scrollType=INCREMENT;
    touchBeginPos = CCPointZero;
    infoPanelStatus=OPEN;
    contentsPanelStatus=CLOSE;
    naviPanelStatus=OPEN;
    adjustY = 792.0;
    detailCicrleWidth = 600.0;
    naviWidth = 460.0;
    moveOffsetY = 30.0;
}

ItemDetailLayer::~ItemDetailLayer()
{
    CC_SAFE_RELEASE(itemList);
    listIndex=0;
    isNaviChange=true;
    listType=PLAN;
    scrollType=INCREMENT;
    touchBeginPos = CCPointZero;
    infoPanelStatus=OPEN;
    contentsPanelStatus=CLOSE;
    naviPanelStatus=OPEN;
    CCLOG("%s","~ItemDetailLayer");

}

bool ItemDetailLayer::init(int idx, ListType type, PlanType plan)
{
    if ( !CCLayer::init() ) {
        return false;
    }
    
    setTouchEnabled(true);
    
    planType=plan;
    listIndex=idx;
    listType=type;
    
    initData();
    initLayout();
    createDetailItem();
    
    scheduleUpdate();
    
    return true;
}

ItemDetailLayer* ItemDetailLayer::create(int idx, ListType type, PlanType plan)
{
    ItemDetailLayer* pRet = new ItemDetailLayer();
    if (pRet && pRet->init(idx, type, plan)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

void ItemDetailLayer::initData()
{
    if ( listType==PLAN ) {
        setItemList(ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate());
        if ( planType==PLAN_BOOKMARK ) {
            setItemList(CreatorsDataFactory::sharedInstance()->getCheckList());
        }
    } else if (listType==SEARCH) {
        setItemList(ViewItemFactory::sharedInstance()->createSearchList());
    } else {
        setItemList(UserItemFactory::sharedInstance()->getUserItemListOrderbyRegdt());

    }
    
}

void ItemDetailLayer::initLayout()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float panelSpace = -5.0;
    
    adjustY = winSize.height/2.0 + 312;
    
    CCSprite* sprite = NULL;
    
    
    //タイトル表示エリアの設定
    sprite = CCSprite::create("nc9228.png");
    CCSize size = sprite->getContentSize();
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+450.0-size.height/2.0));
    addChild(sprite, zOrder_Title, TAG_DETAIL_TITLE);
    
    //内容表示エリアの設定
    OpacitySprite* opSprite = OpacitySprite::createWithSpriteFrameName("bg_detail_content.png");
    contentOpenPoint = ccp(winSize.width/2.0, winSize.height/2.0+350.0-size.height-panelSpace-sprite->getContentSize().height/2.0);
    contentClosePoint =ccp(-273.0, contentOpenPoint.y);
    opSprite->setScale(0.0);
    opSprite->setOpacity(0);
    opSprite->setPosition(contentClosePoint);
    addChild(opSprite, zOrder_ItemContent, TAG_DETAIL_CONTENT);
    
    //ヘッダーメニュー
    sprite = CCSprite::createWithSpriteFrameName("bg_common_menu_v2.png");
    CCSize menuSize = sprite->getContentSize();
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0-450.0 + menuSize.height/2.0));
    addChild(sprite, zOrder_ItemMenu, TAG_DETAIL_MENU);
    
    //アイテムナビエリアの設定
    sprite = CCSprite::createWithSpriteFrameName("bg_detail_itemnavi.png");
    CCSize itemNaviSize = sprite->getContentSize();
    naviOpenPoint = ccp(winSize.width/2.0, winSize.height/2.0-450.0 +menuSize.height+9.0+panelSpace+itemNaviSize.height/2.0);
    naviClosePoint = ccp(winSize.width/2.0, -100.0);
    sprite->setPosition(naviOpenPoint);
    addChild(sprite, zOrder_ItemNavi, TAG_DETAIL_ITEMNAVI);    CCSprite* fsprite = CCSprite::create("bg_detail_itemnavi_filter.png");
    fsprite->setPosition(ccp(sprite->getContentSize().width/2.0, sprite->getContentSize().height/2.0));
    sprite->addChild(fsprite);

    
    //詳細表示エリアの設定
    opSprite = OpacitySprite::createWithSpriteFrameName("bg_detail_info.png");
    infoOpenPoint = ccp(winSize.width/2.0, winSize.height/2.0-433.0 +menuSize.height+panelSpace+itemNaviSize.height + 16.0+panelSpace + sprite->getContentSize().height/2.0);
    infoClosePoint = ccp(winSize.width+273.0, infoOpenPoint.y);
    opSprite->setPosition(infoOpenPoint);
    addChild(opSprite, zOrder_ItemInfo, TAG_DETAIL_INFO);

    //メニュー設定
    sprite = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    //戻るボタン
    CommonButton* button = NULL;
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_detail_returnlist_v2.png"), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::returnListReady));
    button->setTouchPriority(0);
    sprite->addChild(button, 0, TAG_DETAIL_MENU_RETURN_LIST);
    button->setPosition(menuSize.width * 11/12, menuSize.height/2.0);
    
    
    //検索ボタン
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_detail_search.png"), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::openSearch));
    button->setTouchPriority(0);
    sprite->addChild(button);
    button->setPosition(menuSize.width*7/12, menuSize.height/2.0);
    
    //
//    naviCharaMarisaPos =ccp(540, winSize.height/2.0+200);
//    sprite = CCSprite::create("illust_common_marisa_270.png");
//    sprite->setPosition(naviCharaMarisaPos);
//    CCActionInterval* moveDown = CCMoveBy::create(0.5, ccp(0, -20));
//    CCActionInterval* moveUp = CCMoveBy::create(1.0, ccp(0, 20));
//    CCSequence* seq = CCSequence::create(moveDown, moveUp, NULL);
//    sprite->runAction(CCRepeatForever::create(seq));
//    
//    addChild(sprite, zOrder_Title, TAG_DETAIL_NAVICHARA);
    
    //詳細パネル
    initInfoPanelMenu(true);
    
    //内容パネル
    initContentPanelMenu(true);
    
    initNaviPanelMenu(true);
    effectNaviPanel();
    
    //詳細画像のPosition計算
    float asobi = winSize.width/2.0+320 - detailCicrleWidth;
    for (int i=0; i<3; i++) {
        float posX = asobi/2.0 + (detailCicrleWidth/2.0+sin(CC_DEGREES_TO_RADIANS(detailImageDeg[i])) * detailCicrleWidth/2.0);
        float posY = adjustY;
        detailImagePoint[i] = ccp(posX, posY);
    }
    
    //アイテムナビのスケース計算
    for (int i=0; i<3; i++) {
        float scale = (DetailImgSize.width-200 + 200 * cos(CC_DEGREES_TO_RADIANS(detailImageDeg[i])));
        detailImageWidth[i] = scale;
    }
    
    //アイテムナビのPosition計算
    asobi = itemNaviSize.width - naviWidth;
    for (int i=0; i<11; i++) {
        float posX = asobi/2.0 + (naviWidth/2.0+sin(CC_DEGREES_TO_RADIANS(baseDeg[i])) * naviWidth/2.0);
        float posY = itemNaviSize.height/2.0;
        naviItemPoint[i] = ccp(posX, posY);
    }
    //アイテムナビのスケース計算
    for (int i=0; i<11; i++) {
        float scale = (ItenNaviImgSize3.width + ItenNaviImgSize3.width * cos(CC_DEGREES_TO_RADIANS(baseDeg[i])));
        naviItemWidth[i] = scale;
    }
    
}

void ItemDetailLayer::initInfoPanelMenu(bool withAction)
{
    CCSprite* menu = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    if ( menu==NULL ) {
        return;
    }
    CommonButton* button = (CommonButton*)menu->getChildByTag(TAG_DETAIL_MENU_INFO);
    if ( button!=NULL ) {
        menu->removeChild(button, true);
    }
    CCString* imgName = CCString::create("btn_detail_info_off_v2.png");
    if ( infoPanelStatus ) {
        imgName = CCString::create("btn_detail_info_on_v2.png");
    }
    
    CCSize size = menu->getContentSize();

    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName(imgName->getCString()), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::changeInfoPanel));
    button->setTouchPriority(0);
    menu->addChild(button, 0, TAG_DETAIL_MENU_INFO);
    button->setPosition(ccp(size.width*3.0/12.0,size.height/2.0));
}

void ItemDetailLayer::initContentPanelMenu(bool withAction)
{
    CCSprite* menu = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    if ( menu==NULL ) {
        return;
    }
    CommonButton* button = (CommonButton*)menu->getChildByTag(TAG_DETAIL_MENU_CONTENT);
    if ( button!=NULL ) {
        menu->removeChild(button, true);
    }
    CCString* imgName = CCString::create("btn_detail_content_off_v2.png");
    if ( contentsPanelStatus ) {
        imgName = CCString::create("btn_detail_content_on_v2.png");
    }
    
    CCSize size = menu->getContentSize();
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName(imgName->getCString()), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::changeContentPanel));
    button->setTouchPriority(0);
    menu->addChild(button, 0, TAG_DETAIL_MENU_CONTENT);
    button->setPosition(size.width*5/12, size.height/2.0);
    
    contentMenuPosition = ccp(size.width-button->getContentSize().width *5.0/ 2.0 - 42,size.height/2.0);

    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("img_detail_content_button_effect.png");
    
    sprite->setPosition(ccp(button->getContentSize().width/2.0, button->getContentSize().height/2.0));
    button->addChild(sprite, 0, TAG_DETAIL_MENU_CONTENT_NOTICE_1);
    sprite->setScale(0.0);
    sprite->runAction(AnimationFactory::sharedInstance()->getActions("detail-content-notice-1"));
    if ( item->getContents()->length()!=0 ) {
        sprite->setVisible(true);
    } else {
        sprite->setVisible(false);
    }
    

    
}

void ItemDetailLayer::initNaviPanelMenu(bool withAction)
{
    CCSprite* menu = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    if ( menu==NULL ) {
        return;
    }
    CommonButton* button = (CommonButton*)menu->getChildByTag(TAG_DETAIL_MENU_ITEMNAVI);
    if ( button!=NULL ) {
        menu->removeChild(button, true);
    }
    CCString* imgName = CCString::create("btn_detail_navi_off_v2.png");
    if ( naviPanelStatus ) {
        imgName = CCString::create("btn_detail_navi_on_v2.png");
    }
    
    CCSize size = menu->getContentSize();
    
    button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName(imgName->getCString()), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::changeNaviPanel));
    button->setTouchPriority(0);
    menu->addChild(button, 0, TAG_DETAIL_MENU_ITEMNAVI);
    button->setPosition(ccp(size.width*1.0/12.0,size.height/2.0));
}

void ItemDetailLayer::createDetailItem()
{
    CommonButton* button = NULL;
    
    
    isNaviAnim=false;
    if ( listIndex==-1 ) {
        return;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    //背景の位置Sync用のNode作成
    CCNode* node = (CCNode*) getChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE);
    if ( node!=NULL ) {
        node->stopAllActions();
        removeChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE, true);
    }
    
    node = CCNode::create();
    
    node->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    addChild(node, 0, TAG_DETAIL_IMAGE_TARGET_NODE);
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    
    //ナビアイテムの作成
    createItemNavi();
    
    //背景アイテムの作成
    createDetailItemImage(listIndex+1, TAG_DETAIL_IMAGE_TEMP_RIGHT);
    createDetailItemImage(listIndex-1, TAG_DETAIL_IMAGE_TEMP_LEFT);
    createDetailItemImage(listIndex, TAG_DETAIL_IMAGE);
    
    //titleの表示
    CCSprite* sprite = NULL;
    sprite = (CCSprite*)getChildByTag(TAG_DETAIL_TITLE);
    CCSize size = sprite->getContentSize();

    if ( scrollType==INCREMENT ) {
        createDetailItemTitleText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
        createDetailItemTitleText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemTitleText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
    } else {
        createDetailItemTitleText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
        createDetailItemTitleText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemTitleText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
    }

    //発売日判定
    sprite->removeChildByTag(1000, true);
    if ( !DateTimeUtil::isBefore(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
        CCSprite* nowSale = CCSprite::createWithSpriteFrameName("icon_list_nowsale.png");
        nowSale->setPosition(ccp(565, size.height-25));
        sprite->addChild(nowSale, 0, 1000);
        nowSale->setScale(0.9);
//        nowSale->setRotation(20);
        nowSale->runAction(CCFadeIn::create(0.2));
    }
    
    
    //表示アイテムのインデックス表示
    
    //詳細の表示
    if ( scrollType==INCREMENT ) {
        createDetailItemInfoText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
        createDetailItemInfoText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemInfoText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
    } else {
        createDetailItemInfoText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
        createDetailItemInfoText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemInfoText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
    }
    
    //内容表示
    if ( scrollType==INCREMENT ) {
        createDetailItemContentText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
        createDetailItemContentText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemContentText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
    } else {
        createDetailItemContentText(listIndex+1, TAG_DETAIL_FADE_TEXT_RIGHT);
        createDetailItemContentText(listIndex, TAG_DETAIL_FADE_TEXT_CENTER);
        createDetailItemContentText(listIndex-1, TAG_DETAIL_FADE_TEXT_LEFT);
    }
    
    //ContentのNoticeIcon表示
    CCSprite* menu = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    if ( menu==NULL ) {
        return;
    }
    button = (CommonButton*)menu->getChildByTag(TAG_DETAIL_MENU_CONTENT);
    if ( button==NULL ) {
        return;
    }
    
    sprite = dynamic_cast<CCSprite*>(button->getChildByTag(TAG_DETAIL_MENU_CONTENT_NOTICE_1));
    if ( sprite!=NULL ) {
        if ( item->getContents()->length()>0 ) {
            sprite->setVisible(true);
        } else {
            sprite->setVisible(false);
        }
    }
    
    
    //Amazon購入簿ボタン
    menu->removeChildByTag(TAG_DETAIL_MENU_BUY_ITEM, true);
    if ( item->getUrl()!=NULL && item->getUrl()->length() > 0 ) {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_detail_buy_v2.png"), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::buyItem));
        button->setTouchPriority(0);
        button->setPosition(ccp(menu->getContentSize().width*3/4, menu->getContentSize().height/2.0));
        menu->addChild(button,0,TAG_DETAIL_MENU_BUY_ITEM);
    } else {
        CCSprite* buy = CCSprite::createWithSpriteFrameName("btn_detail_buy_v2_noactive.png");
        buy->setPosition(ccp(menu->getContentSize().width*3/4, menu->getContentSize().height/2.0));
        menu->addChild(buy,0,TAG_DETAIL_MENU_BUY_ITEM);
        
    }
    
    
    
    isNaviChange=true;
    
    isNaviAnim=true;
    
    
    //キャラクタ台詞
    SerifLayer* serifLayer2 = dynamic_cast<SerifLayer*>(getChildByTag(TAG_DETAIL_SERIF));
    if ( serifLayer2!=NULL ) {
        serifLayer2->removeSerif(0.0);
    }
    CCString* serif = NULL;
    if ( listType!=BOOKMARK && item->getItemid()->length()!=0 && isContain(item->getItemid(), 3) ) {
        serif = CCString::createWithFormat("作品名：『%s』はブックマークに登録されています。", item->getTitle()->getCString());
    } else if ( listType!=BOOKMARK && listType!=SEARCH && item->getAuther()->length()!=0 &&isContain(item->getAuther(), 1) ) {
        serif = CCString::createWithFormat("作家名：『%s』はブックマークに登録されています。", item->getAuther()->getCString());
    } else if ( listType!=BOOKMARK && listType!=SEARCH && item->getCreator()->length()!=0 && isContain(item->getCreator(), 2) ) {
        serif = CCString::createWithFormat("イラスト名：『%s』はブックマークに登録されています。", item->getCreator()->getCString());
    }
    if ( serif!=NULL ) {
        SerifLayer* serifLayer = SerifLayer::create();
        serifLayer->setTextString(serif->getCString());
        addChild(serifLayer, zOrder_Title, TAG_DETAIL_SERIF);
//        ItemViewScene* scene = (ItemViewScene*)CCDirector::sharedDirector()->getRunningScene();
//        scene->createSerif(serif->getCString(), false);
    }
    
    
//    pthread_mutex_lock(&s_mutex);
//    threadData = new THREADDATA();
//    threadData->dic = new CCDictionary();
//    threadData->list = itemList;
//    threadData->index=listIndex;
//    pthread_mutex_unlock(&s_mutex);
//
//    
//    //ワーカースレッド追加
//    pthread_mutex_init(&s_mutex, NULL);
//    pthread_create(&s_thread, NULL, workerThread, (void*)threadData);
//    pthread_detach(s_thread);
}

void *ItemDetailLayer::workerThread(void *arg)
{
    CCLOG("%s", "####################Start#############");
    THREADDATA* td = (THREADDATA*)arg;
    pthread_mutex_lock(&s_mutex);
    CCLog("Thread List %d", td->list->count());
    CCLog("Thread ListIndex %d", td->index);

    pthread_mutex_unlock(&s_mutex);
    CCLOG("%s", "#####################End##############");
    return NULL;
};


void ItemDetailLayer::createDetailItemImage(int idx, int tag)
{
    int baseTag = TAG_DETAIL_IMAGE_TEMP_LEFT;
    int getIndex = tag-baseTag;
    

    
    CCSprite* sprite = (CCSprite*)getChildByTag(tag);
    if ( sprite!=NULL ) {
        sprite->stopAllActions();
        removeChildByTag(tag, true);
    }
    
    //indexの数がOutboundの場合は抜ける
    if ( idx>itemList->count()-1 ) {
        return;
    }
    
    //indexの数がOutboundの場合は抜ける
    if ( idx<0 ) {
        return;
    }

    //ここでは直接読み込まずに、NaviItemから取得する。
    int targetTag = TAG_DETAIL_ITEMNAVI_ITEM_6; //真ん中
    if ( tag==TAG_DETAIL_IMAGE_TEMP_LEFT ) targetTag=TAG_DETAIL_ITEMNAVI_ITEM_5;
    if ( tag==TAG_DETAIL_IMAGE_TEMP_RIGHT ) targetTag=TAG_DETAIL_ITEMNAVI_ITEM_7;
    
    CCSprite* itemNavi = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
    if ( itemNavi==NULL ) {
        return;
    }
    
    CCSprite* naviSprite = (CCSprite*)itemNavi->getChildByTag(targetTag);
    
    
    sprite = CCSprite::createWithTexture(naviSprite->getTexture());
    
    if (strcmp("Y", ((CCString*)naviSprite->getUserObject())->getCString()) ) {
        CCTextureCache* cache = CCTextureCache::sharedTextureCache();
        sprite = CCSprite::createWithTexture(cache->textureForKey("img_detail_noimage_bg.png"));
//        sprite = CCSprite::create("img_detail_noimage_bg.png");
    }
    CCLOG("tag  %s", ((CCString*)naviSprite->getUserObject())->getCString());
    sprite->setOpacity(detailOpacity[getIndex]);
    CCSize spriteSize = sprite->getContentSize();
    sprite->setAnchorPoint(ccp(0.5,1.0));
    
    if( true ) {
        sprite->setScale(detailImageWidth[getIndex]/ spriteSize.width);
    }
    sprite->setPosition(detailImagePoint[getIndex]);

    addChild(sprite, zOrder_ItemImage, tag);
    
}

void ItemDetailLayer::createDetailItemTitleText(int index, int tag)
{
    CCLabelTTF* label = NULL;
    CCSprite* sprite = dynamic_cast<CCSprite*>(getChildByTag(TAG_DETAIL_TITLE));
    
    if ( sprite==NULL ) return;
    if (index==-1) return;

    CCSize size = sprite->getContentSize();
    
    //左のタイトル作成
    if ( index<0 ) return;
    if ( index>=itemList->count() ) return;
    
    //ラベルがすでにあるならそれを使おう改修
    if ( createDetailText(TAG_DETAIL_TITLE, tag) ) {
        CCLog("%s", "Create Title From Tag");
        return;
    }
    CCLog("%s", "Create Title From Label");
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    bool isContainItem = isContain(item->getItemid(), 3);
    label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 28, CCSizeMake(420, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    if ( isContainItem ) {
        label->setColor(ccc3(255, 255, 0));
    }
    label->setPosition(ccp(size.width/2.0, size.height/2.0+5));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->runAction(CCFadeIn::create(0.2));
    sprite->addChild(label, 0, tag);
    
    CCString* typeStr = CCString::create("発売予定");
    if ( listType==SEARCH ) typeStr = CCString::create("検索結果");
    if ( listType==BOOKMARK ) typeStr = CCString::create("ブックマーク");
    CCString* labelIndexStr = CCString::createWithFormat("%s 全%d件中 %d件目", typeStr->getCString(), itemList->count(),index+1);
    label = CCLabelTTF::create(labelIndexStr->getCString(), FONT_W6, 18, CCSizeMake(400, 30), kCCTextAlignmentRight, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(size.width-350.0, 25.0));
    label->setColor(ccc3(255, 255, 0));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->runAction(CCFadeIn::create(0.2));
    sprite->addChild(label, 0, tag);

}


bool ItemDetailLayer::createDetailText(int parentTag, int createTag) {

    CCArray* ary = CCArray::create();
    if ( parentTag==TAG_DETAIL_TITLE ) {
        CCSprite* sprite = dynamic_cast<CCSprite*>(getChildByTag(parentTag));
        if ( sprite==NULL ) return false;
        ary = sprite->getChildren();
    } else {
        OpacitySprite* sprite = dynamic_cast<OpacitySprite*>(getChildByTag(parentTag));
        if ( sprite==NULL ) return false;
        ary = sprite->getChildren();
    }
    
    if ( ary==NULL || ary->count()==0 ) return false;
    
    //Tagの削除　removeChildByTagdでは複数の場合１つしか削除しないので。。
    CCArray* removeAry = CCArray::create();
    for (int i=0; i<ary->count(); i++) {
        CCNode* node = (CCNode*)ary->objectAtIndex(i);
        if ( node->getTag()==createTag ) {
            removeAry->addObject(node);
        }
    }
    
    //For文の中で削除するとindexがおかしくなる。
    if ( removeAry->count()!=0 ) {
        for (int i=0; i<removeAry->count(); i++) {
            CCNode* node = (CCNode*)removeAry->objectAtIndex(i);
            node->removeFromParentAndCleanup(true);
        }
    }
    
    if ( scrollType==INCREMENT && createTag== TAG_DETAIL_FADE_TEXT_RIGHT) return false;
    if ( scrollType==DECREMENT && createTag== TAG_DETAIL_FADE_TEXT_LEFT) return false;
    
    int targetTag = TAG_DETAIL_FADE_TEXT_CENTER;
    
    switch (scrollType) {
        case INCREMENT:
            if (createTag==TAG_DETAIL_FADE_TEXT_CENTER) targetTag = TAG_DETAIL_FADE_TEXT_RIGHT;
            break;
        case DECREMENT:
            if (createTag==TAG_DETAIL_FADE_TEXT_CENTER) targetTag = TAG_DETAIL_FADE_TEXT_LEFT;
            break;
        default:
            break;
    }
    bool returnFlg = false;
    for (int i=0; i<ary->count(); i++) {
        CCNode* label = (CCNode*)ary->objectAtIndex(i);
        if ( label==NULL ) {
            continue;
        }
        if ( label->getTag()==targetTag ) {
            label->setTag(createTag);
            returnFlg = true;
        }
        CommonButton* button = dynamic_cast<CommonButton*>(ary->objectAtIndex(i));
        if ( button!=NULL && button->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
            button->setEnabled(true);
        } else if (button!=NULL && button->getTag()!=TAG_DETAIL_FADE_TEXT_CENTER) {
            button->setEnabled(false);
        }
    }
    return returnFlg;
}

void ItemDetailLayer::createDetailItemInfoText(int index, int tag)
{

    CCLabelTTF* label = NULL;
    OpacitySprite* opSprite = dynamic_cast<OpacitySprite*>(getChildByTag(TAG_DETAIL_INFO));
    
    if ( opSprite==NULL ) return;
    if (index==-1) return;
    
    CCSize size = opSprite->getContentSize();
    
    //左のタイトル作成
    if ( index<0 ) return;
    if ( index>=itemList->count() ) return;
    
    //ラベルがすでにあるならそれを使おう改修
    if ( createDetailText(TAG_DETAIL_INFO, tag) ) {
        return;
    }
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    
    //各種フラグ設定
    bool isContainItem = isContain(item->getItemid(), 3);
    bool isContainAuthor = isContain(item->getAuther(), 1);
    bool isContainCreator = isContain(item->getCreator() , 2);

    //↓Infoマーク↓
    if ( !isContainItem && listType!=BOOKMARK && listType!=SEARCH && ((item->getAuther()->length()!=0 && isContainAuthor) || (item->getCreator()->length()!=0 && isContainCreator)) ) {
        
        CommonButton *button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_detail_bookmark_info_2.png"), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::openBookmarkInfo));
        button->setPosition(ccp(size.width-115, size.height/2.0+43.0));
        button->setOpacity(opSprite->getOpacity());
        opSprite->addChild(button, 0, tag);

        
        if ( listIndex!=index ) {
            button->setEnabled(false);
        }
        
        CCSprite* sprite = CCSprite::createWithSpriteFrameName("btn_detail_bookmark_info_1.png");
        sprite->setPosition(ccp(size.width-115, size.height/2.0+43.0));
        sprite->setOpacity(opSprite->getOpacity());
        sprite->runAction(CCFadeIn::create(0.2));
        opSprite->addChild(sprite, 0, tag);
    }
    
    CCString* str = CCString::createWithFormat("作家　　：%s", item->getAuther()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 24, CCSizeMake(400, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(size.width/2.0, size.height-40));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->setOpacity(opSprite->getOpacity());

    opSprite->addChild(label, 0, tag);
    if ( isContainItem || isContainAuthor ) {
        label->setColor(ccc3(255, 255, 0));
    }
    

    str = CCString::createWithFormat("イラスト：%s", item->getCreator()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 24, CCSizeMake(400, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(size.width/2.0, size.height-70));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->setOpacity(opSprite->getOpacity());

    opSprite->addChild(label, 0, tag);
    if ( isContainItem || (isContainCreator && item->getCreator()->length()!=0) ) {
        label->setColor(ccc3(255, 255, 0));
    }

    
    str = CCString::createWithFormat("レーベル：%s", item->getLabel()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 24, CCSizeMake(400, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(size.width/2.0, size.height-100));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->setOpacity(opSprite->getOpacity());
    if ( isContainItem ) {
        label->setColor(ccc3(255, 255, 0));
    }
    
    opSprite->addChild(label, 0, tag);

    
    str = CCString::createWithFormat("発売日　：%s", item->getReleaseDate()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 24, CCSizeMake(400, 30), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(size.width/2.0, size.height-130));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->setOpacity(opSprite->getOpacity());
    if ( isContainItem ) {
        label->setColor(ccc3(255, 255, 0));
    }
    label->visit();
    opSprite->addChild(label, 0, tag);

}

void ItemDetailLayer::createDetailItemContentText(int index, int tag)
{
    CCLabelTTF* label = NULL;
    OpacitySprite* opSprite = dynamic_cast<OpacitySprite*>(getChildByTag(TAG_DETAIL_CONTENT));
    
    if ( opSprite==NULL ) return;
    if (index==-1) return;
    
    CCSize size = opSprite->getContentSize();
    
    //左のタイトル作成
    if ( index<0 ) return;
    if ( index>=itemList->count() ) return;
    
    bool isCreateTag = createDetailText(TAG_DETAIL_CONTENT, tag);

    if ( isCreateTag ) {
        return;
    }
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    
    
    CCString *str = CCString::createWithFormat("%s", item->getContents()->getCString());
    if ( str->length()==0 ) {
        str = CCString::create("この作品の概要はまだありません。");
    }
    CCSize labelSizeFull = CCSizeMake(420, size.height-60);
    CCSize labelSizeSmall = CCSizeMake(420, size.height-100);
    
    
    
    
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 22, labelSizeSmall, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    
    label->setDimensions(CCSizeMake(420, 0));
    label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
    
    
    if ( labelSizeFull.height < label->getContentSize().height ) {
        CommonButton* button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_detail_content_all_open.png"), CCSizeZero, this, callfuncO_selector(ItemDetailLayer::openContentAll));
        button->setPosition(ccp(size.width/2.0, 40));
        button->setOpacity(opSprite->getOpacity());

        if ( opSprite->getOpacity()>=200 ) {
            button->runAction(CCFadeIn::create(2.0));
        }
        opSprite->addChild(button, 0, tag);
        
        if ( listIndex!=index ) {
            button->setEnabled(false);
        }
        
        label->setDimensions(CCSizeMake(label->getContentSize().width, labelSizeSmall.height));
    }
    
    
    label->setAnchorPoint(ccp(0.5,1.0));
    label->setPosition(ccp(size.width/2.0, size.height-30.0));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    //    label->runAction(CCFadeIn::create(0.2));
    label->setOpacity(opSprite->getOpacity());
    if ( opSprite->getOpacity()>=200 ) {
        label->runAction(CCFadeIn::create(0.2));
    }
    opSprite->addChild(label, 0, tag);
}

void ItemDetailLayer::createItemNavi()
{
    CCSprite* itemNavi = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
    if ( itemNavi==NULL ) {
        return;
    }
    if ( !isNaviChange ) {
        return;
    }
//    itemNavi->removeAllChildrenWithCleanup(true);

    CCSize itenNaviSize = itemNavi->getContentSize();

    CCSprite* label = CCSprite::createWithSpriteFrameName("label_detail_itemnavi.png");
    label->setPosition(ccp(itenNaviSize.width/2.0, itenNaviSize.height/2.0));
    itemNavi->addChild(label);
    
    //Texture
    CCDictionary* dic = CCDictionary::create();
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_1) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_1), TAG_DETAIL_ITEMNAVI_ITEM_1);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_2) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_2), TAG_DETAIL_ITEMNAVI_ITEM_2);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_3) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_3), TAG_DETAIL_ITEMNAVI_ITEM_3);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_4) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_4), TAG_DETAIL_ITEMNAVI_ITEM_4);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_5) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_5), TAG_DETAIL_ITEMNAVI_ITEM_5);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_6) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_6), TAG_DETAIL_ITEMNAVI_ITEM_6);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_7) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_7), TAG_DETAIL_ITEMNAVI_ITEM_7);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_8) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_8), TAG_DETAIL_ITEMNAVI_ITEM_8);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_9) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_9), TAG_DETAIL_ITEMNAVI_ITEM_9);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_10) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_10), TAG_DETAIL_ITEMNAVI_ITEM_10);
    }
    if ( itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_11) ) {
        dic->setObject(itemNavi->getChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_11), TAG_DETAIL_ITEMNAVI_ITEM_11);
    }
    itemNavi->removeAllChildrenWithCleanup(true);
    
    CCSprite* fsprite = CCSprite::create("bg_detail_itemnavi_filter.png");
    fsprite->setPosition(ccp(itemNavi->getContentSize().width/2.0, itemNavi->getContentSize().height/2.0-4.0));
    itemNavi->addChild(fsprite, 1000);

    
    if ( scrollType==INCREMENT ) {
        removeChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_11, true);
        //ナビアイテム　右側
        createItemNaviCommon(listIndex+5, TAG_DETAIL_ITEMNAVI_ITEM_11, dic);
        createItemNaviCommon(listIndex+4, TAG_DETAIL_ITEMNAVI_ITEM_10, dic);
        createItemNaviCommon(listIndex+3, TAG_DETAIL_ITEMNAVI_ITEM_9, dic);
        createItemNaviCommon(listIndex+2, TAG_DETAIL_ITEMNAVI_ITEM_8, dic);
        createItemNaviCommon(listIndex+1, TAG_DETAIL_ITEMNAVI_ITEM_7, dic);
        createItemNaviCommon(listIndex,  TAG_DETAIL_ITEMNAVI_ITEM_6, dic);
        
        //ナビアイテム　左側
        createItemNaviCommon(listIndex-1, TAG_DETAIL_ITEMNAVI_ITEM_5, dic);
        createItemNaviCommon(listIndex-2, TAG_DETAIL_ITEMNAVI_ITEM_4, dic);
        createItemNaviCommon(listIndex-3, TAG_DETAIL_ITEMNAVI_ITEM_3, dic);
        createItemNaviCommon(listIndex-4, TAG_DETAIL_ITEMNAVI_ITEM_2, dic);
        createItemNaviCommon(listIndex-5, TAG_DETAIL_ITEMNAVI_ITEM_1, dic);
    } else {
        removeChildByTag(TAG_DETAIL_ITEMNAVI_ITEM_1, true);
        //ナビアイテム　左側
        createItemNaviCommon(listIndex-5, TAG_DETAIL_ITEMNAVI_ITEM_1, dic);
        createItemNaviCommon(listIndex-4, TAG_DETAIL_ITEMNAVI_ITEM_2, dic);
        createItemNaviCommon(listIndex-3, TAG_DETAIL_ITEMNAVI_ITEM_3, dic);
        createItemNaviCommon(listIndex-2, TAG_DETAIL_ITEMNAVI_ITEM_4, dic);
        createItemNaviCommon(listIndex-1, TAG_DETAIL_ITEMNAVI_ITEM_5, dic);
        //ナビアイテム　カレント
        createItemNaviCommon(listIndex,  TAG_DETAIL_ITEMNAVI_ITEM_6, dic);
        //ナビアイテム　右側
        createItemNaviCommon(listIndex+1, TAG_DETAIL_ITEMNAVI_ITEM_7, dic);
        createItemNaviCommon(listIndex+2, TAG_DETAIL_ITEMNAVI_ITEM_8, dic);
        createItemNaviCommon(listIndex+3, TAG_DETAIL_ITEMNAVI_ITEM_9, dic);
        createItemNaviCommon(listIndex+4, TAG_DETAIL_ITEMNAVI_ITEM_10, dic);
        createItemNaviCommon(listIndex+5, TAG_DETAIL_ITEMNAVI_ITEM_11, dic);


    }
}

void ItemDetailLayer::createItemNaviCommon(int idx, int tag, CCDictionary* dic)
{
    CCSprite* itemNavi = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
    if ( itemNavi==NULL ) {
        return;
    }
    
    //indexの数がOutboundの場合は抜ける
    if ( idx>itemList->count()-1 ) {
        return;
    }
    
    //indexの数がOutboundの場合は抜ける
    if ( idx<0 ) {
        return;
    }
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(idx);
    
    CCSprite* spriteByTag = NULL;
    if ( scrollType==INCREMENT ) {
        spriteByTag = (CCSprite*)dic->objectForKey(tag+1);
    } else {
        spriteByTag = (CCSprite*)dic->objectForKey(tag-1);
    }
    
    CCSprite* sprite = NULL;
    
    if ( spriteByTag==NULL ) {
        CCImage *image = new CCImage();
        std::string imagePath =UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "L_" +item->getItemid()->getCString();
//        if ( listType==SEARCH ) {
//            imagePath=UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "L_" +item->getItemid()->getCString();
//        }
        bool flg = image->initWithImageFile(imagePath.c_str());
        if ( !flg ) {
            image->initWithImageFile("img_detail_noimage.png");
        }
        image->autorelease();
        // 3. CCTexture2DからCCSpriteへ
        CCTexture2D *texture = new CCTexture2D();
        texture->initWithImage(image);
        texture->autorelease();
        sprite = CCSprite::createWithTexture(texture);
        (flg) ? sprite->setUserObject(CCString::create("Y")):sprite->setUserObject(CCString::create("N"));
        CCLOG("%d is created by CCImage",tag);
    } else {
        CCLOG("%d is created by Tag Texture",tag);
        sprite = CCSprite::createWithTexture(spriteByTag->getTexture());
        sprite->setUserObject(spriteByTag->getUserObject());
    }
    
    CCSize spriteSize = sprite->getContentSize();
    sprite->setScaleX(naviItemWidth[tag-TAG_DETAIL_ITEMNAVI_ITEM_1] / spriteSize.width);
    sprite->setScaleY(naviItemWidth[tag-TAG_DETAIL_ITEMNAVI_ITEM_1]/ spriteSize.height);
    sprite->setPosition(naviItemPoint[tag-TAG_DETAIL_ITEMNAVI_ITEM_1]);
    int zOrder = 6;
    switch (tag) {
        case TAG_DETAIL_ITEMNAVI_ITEM_11:
        case TAG_DETAIL_ITEMNAVI_ITEM_1:
            zOrder=1;
            break;
        case TAG_DETAIL_ITEMNAVI_ITEM_10:
        case TAG_DETAIL_ITEMNAVI_ITEM_2:
            zOrder=2;
            break;
        case TAG_DETAIL_ITEMNAVI_ITEM_9:
        case TAG_DETAIL_ITEMNAVI_ITEM_3:
            zOrder=3;
            break;
        case TAG_DETAIL_ITEMNAVI_ITEM_8:
        case TAG_DETAIL_ITEMNAVI_ITEM_4:
            zOrder=4;
            break;
        case TAG_DETAIL_ITEMNAVI_ITEM_7:
        case TAG_DETAIL_ITEMNAVI_ITEM_5:
            zOrder=5;
            break;
        default:
            break;
    }
    
    itemNavi->addChild(sprite, zOrder, tag);
    if ( idx==listIndex ) {
        CCSprite* fSprite = CCSprite::createWithTexture(sprite->getTexture());
        CCSize spriteSize = fSprite->getContentSize();
//        fSprite->setScaleX(naviItemWidth[tag-TAG_DETAIL_ITEMNAVI_ITEM_1] / spriteSize.width);
//        fSprite->setScaleY(naviItemWidth[tag-TAG_DETAIL_ITEMNAVI_ITEM_1]/ spriteSize.height);
//        fSprite->setPosition(naviItemPoint[tag-TAG_DETAIL_ITEMNAVI_ITEM_1]);
        
        CCSpawn* spawn = CCSpawn::create(CCScaleTo::create(0.6, 1.3), CCFadeTo::create(0.6, 0), NULL);
        CCCallFunc* func = CCCallFunc::create(fSprite, callfunc_selector(CCSprite::removeFromParent));
        
        
        fSprite->runAction(CCSequence::create(CCDelayTime::create(0.1), spawn, func, NULL));
        fSprite->setPosition(ccp(sprite->getContentSize().width/2.0, sprite->getContentSize().height/2.0));
        sprite->addChild(fSprite);

    }
}

void ItemDetailLayer::removeBGTemp()
{
    CCSprite* bgTemp = (CCSprite*)getChildByTag(TAG_DETAIL_IMAGE_TEMP_RIGHT);
    if ( bgTemp!=NULL ) {
        bgTemp->stopAllActions();
        removeChild(bgTemp, true);
    }
    
    
};

void ItemDetailLayer::panelManage(int panelType)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* panel = NULL;
    OpacitySprite* opPanel = NULL;
    switch (panelType) {
        case 0:
        {
            CCString* actionStr = (infoPanelStatus==CLOSE) ? CCString::create("Detail-Navi-in") : CCString::create("Detail-Navi-Out");
            CCPoint movePoint = (infoPanelStatus==OPEN) ? infoClosePoint: infoOpenPoint;
            opPanel = (OpacitySprite*)getChildByTag(TAG_DETAIL_INFO);
            opPanel->stopAllActions();
//            panel->runAction(CCMoveTo::create(0.5, movePoint));
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCFloat::create(movePoint.x), "move_posx");
            dic->setObject(CCFloat::create(movePoint.y), "move_posy");
            CCActionInterval* action = NULL;
            if ( infoPanelStatus==OPEN ) {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic, CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::doBackGroundEffectInfo)));
            } else {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic);
            }
            
            action->setTag(TAG_DETAIL_ANIMATION_PANELMANAGE);
            opPanel->runAction(action);
 
            infoPanelStatus =(infoPanelStatus==OPEN) ? CLOSE: OPEN;
            initInfoPanelMenu(false);
            
            CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
            if ( charactor!=NULL ) {
                if ( infoPanelStatus==OPEN ) {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_PURPLE, MagicCircleEffectStatus::StarMoveType_OUT);
                } else {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_PURPLE, MagicCircleEffectStatus::StarMoveType_IN);
                }
            }
            
            if ( infoPanelStatus==CLOSE ) return;
            
            doBackGroundEffectInfo();
            
            
            
            
        }
            break;
        case 1:
        {
            CCString* actionStr = (contentsPanelStatus==CLOSE) ? CCString::create("Detail-Navi-in") : CCString::create("Detail-Navi-Out");
            CCPoint movePoint = (contentsPanelStatus==OPEN) ? contentClosePoint: contentOpenPoint;
            opPanel = (OpacitySprite*)getChildByTag(TAG_DETAIL_CONTENT);
            opPanel->stopAllActions();
//            panel->runAction(CCMoveTo::create(0.5, movePoint));
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCFloat::create(movePoint.x), "move_posx");
            dic->setObject(CCFloat::create(movePoint.y), "move_posy");
            CCActionInterval* action = NULL;
            if ( contentsPanelStatus==OPEN ) {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic, CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::doBackGroundEffectContent)));
                
            } else {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic);
            }
            
            action->setTag(TAG_DETAIL_ANIMATION_PANELMANAGE);
            opPanel->runAction(action);
            contentsPanelStatus =(contentsPanelStatus==OPEN) ? CLOSE: OPEN;
            initContentPanelMenu(false);
            
            CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
            if ( charactor!=NULL ) {
                if ( contentsPanelStatus==OPEN ) {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_GREEN, MagicCircleEffectStatus::StarMoveType_OUT);
                    charactor->moveLayer(ccp(+70.0,0), 0.5);
                    charactor->isMove = false;
                } else if ( contentsPanelStatus==CLOSE ) {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_GREEN,MagicCircleEffectStatus::StarMoveType_IN);
                    charactor->isMove = true;
                    charactor->moveLayer(ccp(-70.0,0), 0.5);
                }
            }

            if ( contentsPanelStatus==CLOSE ) return;
            
            doBackGroundEffectContent();
            
        }
            break;
        case 2:
        {
            CCPoint movePoint = (naviPanelStatus==OPEN) ? naviClosePoint: naviOpenPoint;
            CCString* actionStr = (naviPanelStatus==CLOSE) ? CCString::create("Detail-Navi-in") : CCString::create("Detail-Navi-Out");
            panel = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
            panel->stopAllActions();
            CCActionInterval* action = NULL;
            CCDictionary* dic = CCDictionary::create();
            dic->setObject(CCFloat::create(movePoint.x), "move_posx");
            dic->setObject(CCFloat::create(movePoint.y), "move_posy");

            if ( naviPanelStatus==OPEN ) {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic, CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::doBackGroundEffectNavi)));
            } else {
                action = AnimationFactory::sharedInstance()->getActions(actionStr->getCString(), dic, CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::effectNaviPanel)));
            }

            action->setTag(TAG_DETAIL_ANIMATION_PANELMANAGE);
            panel->runAction(action);
            naviPanelStatus =(naviPanelStatus==OPEN) ? CLOSE: OPEN;
            initNaviPanelMenu(false);

            CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
            if ( charactor!=NULL ) {
                if (naviPanelStatus==OPEN) {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_BLUE, MagicCircleEffectStatus::StarMoveType_OUT);
                } else {
                    charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_BLUE, MagicCircleEffectStatus::StarMoveType_IN);
                }
            }

            
            if ( naviPanelStatus==CLOSE ) return;
            

            
            doBackGroundEffectNavi();
            
        }
            break;
            
        default:
            break;
    }
}

void ItemDetailLayer::changeInfoPanel()
{
    panelManage(0);
}

void ItemDetailLayer::changeContentPanel()
{
    panelManage(1);
}

void ItemDetailLayer::changeNaviPanel()
{
    panelManage(2);
}

void ItemDetailLayer::effectNaviPanel()
{
    if ( naviPanelStatus==CLOSE ) return;
    CCSprite* panel = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
    CCNode* node = CCNode::create();
    addChild(node,zOrder_Title);
    
    CCSize size = panel->getContentSize();
    CCPoint point = panel->getPosition();
    
    node->setPosition(ccp(size.width/2.0 ,point.y));
    
    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("ListChangeEffect.plist");
    particle->setPosition(CCPointZero);
    particle->setDuration(0.4);
    particle->setAutoRemoveOnFinish(true);
//    node->addChild(particle);
    node->runAction(CCMoveTo::create(0.4, ccp(0.0,point.y)));

    node = CCNode::create();
    addChild(node,zOrder_Title);
    
    
    node->setPosition(ccp(size.width/2.0 ,point.y));
    
    CCParticleSystemQuad *particle2 = CCParticleSystemQuad::create("ListChangeEffect.plist");
    particle2->setPosition(CCPointZero);
    particle2->setDuration(0.4);
    particle2->setAutoRemoveOnFinish(true);
//    node->addChild(particle2);
    node->runAction(CCMoveTo::create(0.4, ccp(size.width,point.y)));
}

void ItemDetailLayer::returnBgPosition()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCNode* node = (CCNode*)getChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE);
    node->runAction(CCMoveTo::create(0.2, ccp(winSize.width/2.0,node->getPosition().y)));

}

void ItemDetailLayer::doBackGroundEffect(int bgType)
{
    ((ItemViewScene*)getParent())->doBackGroundEffect(bgType);
}

void ItemDetailLayer::openSearch()
{
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(item->getAuther(), "author");
    dic->setObject(item->getCreator(), "creator");
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_Search
                        , scene, callfunc_selector(ItemViewScene::changeViewSearch), dic);
}

void ItemDetailLayer::openContentAll()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    OpacitySprite* opSprite = (OpacitySprite*)getChildByTag(TAG_DETAIL_CONTENT);
    CCSize opSize = opSprite->getContentSize();

    opSprite = (OpacitySprite*)getChildByTag(TAG_DETAIL_TITLE);
    CCSize titleSize = opSprite->getContentSize();
    
    CCPoint titlePos = ccp(winSize.width/2.0, winSize.height/2.0+450.0-titleSize.height/2.0+5);
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);

    if ( getChildByTag(TAG_DETAIL_CONTENT_BG)!=NULL ) {
        removeChildByTag(TAG_DETAIL_CONTENT_BG, true);
    }
    
    BgLayer* bgLayer = BgLayer::create();
    CCPoint pos = bgLayer->getPosition();
    bgLayer->setPosition(ccp(pos.x, pos.y - winSize.height));
    bgLayer->runAction(CCMoveTo::create(0.5, CCPointZero));
    addChild(bgLayer, zOrder_ItemContent_Back, TAG_DETAIL_CONTENT_BG);

    CCSprite* sprite = CCSprite::create("frameback.png");
    sprite->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    sprite->setOpacity(0);
    sprite->runAction(CCSequence::create(CCDelayTime::create(0.5), CCFadeIn::create(0.5), NULL));

    bgLayer->addChild(sprite);

    
    
    CCLabelTTF* label = NULL;
    label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 28, CCSizeMake(420, 100), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(titlePos);
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    label->setOpacity(0);
    label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    
    bgLayer->addChild(label,3);
    
    CCSize labelSizeSmall = CCSizeMake(420, 100);
    label = CCLabelTTF::create(item->getContents()->getCString(), FONT_W6, 22, labelSizeSmall, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    
    label->setDimensions(CCSizeMake(420, 0));
    label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
    
    label->setAnchorPoint(ccp(0.5, 1.0));
    label->setPosition(ccp(winSize.width/2.0, contentOpenPoint.y + opSize.height/2.0-30));
    label->setOpacity(0);
    label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    
    
    bgLayer->addChild(label,3);

    //キャラクターレイヤの追加
    CharactorLayer* charactorLayer = dynamic_cast<CharactorLayer*>(getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));

    if ( charactorLayer!=NULL ) {
        CCActionInterval* act1 = CCMoveBy::create(0.5, ccp(0.0, winSize.height));
        CCEaseBackIn* ease1 = CCEaseBackIn::create(act1);
        charactorLayer->runAction(ease1);
    }
    
    Charactor* charactor = Charactor::create("Charactor_h_350_shadow.png");
    charactor->setPosition(ccp(winSize.width/2.0+100, -winSize.height*1.5));
    charactor->setOpacity(0);
    charactor->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    
    bgLayer->addChild(charactor);
    
    SubCharactor* subCharactor = SubCharactor::create(ccp(160,210));
    subCharactor->setOpacity(0);
    subCharactor->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    subCharactor->deleteNotification();

    charactor->addChild(subCharactor);
    
    sprite = CCSprite::createWithSpriteFrameName("img_serif_detail_tap_close.png");
    sprite->setPosition(ccp(80, 300));
    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(1.0), CCEaseBackOut::create(CCMoveTo::create(1.0, ccp(winSize.width/2.0+100, winSize.height/2.0-300))), NULL);
    charactor->runAction(seq2);
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCDelayTime* delay2 = CCDelayTime::create(0.5);
    CCFadeOut* fadeout = CCFadeOut::create(0.5);
    CCFadeIn* fadein = CCFadeIn::create(0.0);
    CCSequence* seq = CCSequence::create(delay, fadeout, delay2, fadein, NULL);
    sprite->runAction(CCRepeatForever::create(seq));
    charactor->addChild(sprite);

    
//    
//    charactor = CharactorLayer::create();
//    charactor->setScaleX(0.1);
//    CCActionInterval* scale = CCScaleTo::create(0.5, 1.0, 1.0);
//    charactor->runAction(scale);
//    getParent()->addChild(charactor, ItemViewScene::zOrder_Charactor, TAG_ITEMVIEW_CHARACTOR_SUB);
    
}


void ItemDetailLayer::buyItem()
{
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    if ( item->getUrl()!=NULL && item->getUrl()->length()<1 ) {
        return;
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    Cocos2dExt::BrowserLauncher::launchUrl(item->getUrl()->getCString());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif

}

void ItemDetailLayer::returnListReady()
{
    CCSprite* menuSprite = (CCSprite*)getChildByTag(TAG_DETAIL_MENU);
    if ( menuSprite==NULL ) {
        return;
    }
    CommonButton* button = (CommonButton*)menuSprite->getChildByTag(TAG_DETAIL_MENU_RETURN_LIST);
    if ( button == NULL ) {
        return;
    }
    
    button->setEnabled(false);
    
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->isMove=true;
        charactor->resetPosition();
        charactor->createMagicCircleEffect(MagicCircleEffectStatus::ColorType_NONE, MagicCircleEffectStatus::StarMoveType_OUT);
    }    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    
    CCSprite* sprite = CCSprite::createWithSpriteFrameName("img_star_black.png");
    sprite->setPosition(ccp(winSize.width/2.0+130, winSize.height/2.0+180.0));
    sprite->setScale(0.0);
    CCDirector::sharedDirector()->getRunningScene()->addChild(sprite, 1000);
    CCActionInterval* scale1 = CCScaleTo::create(1.0, 20);
    CCEaseIn* ease = CCEaseIn::create(scale1, 2.0);
    CCSequence* seq = CCSequence::create(ease, CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::returnList)), CCCallFunc::create(sprite, callfunc_selector(CCSprite::removeFromParent)), NULL);
        sprite->runAction(seq);
}

void ItemDetailLayer::returnList()
{
    CCLog("Return List");
    ItemViewScene* scene = (ItemViewScene*) getParent();
    if ( scene==NULL ) {
        return;
    }
    scene->setListIndex(listIndex);
    scene->setViewType(LIST);
    scene->doViewProcess();
    scene->createDialog(DialogLayer::DialogType_ReturnList
                        , NULL, NULL, NULL);
}


void ItemDetailLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

bool ItemDetailLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    panelFadeOut();
    touchBeginPos = pTouch->getLocation();
    return true;
}

void ItemDetailLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    panelFadeIn();
    CharactorStatus::AnimationStatus animation = CharactorStatus::AnimationStatus_Rotate_left;
    float disX = touchBeginPos.x - pTouch->getLocation().x;
    float disY = touchBeginPos.y - pTouch->getLocation().y;
    if ( abs((int)disX) < abs((int)disY) ) {
        upFlick(pTouch);
        return;
    }
    CCLOG("%d",abs(touchBeginPos.x-pTouch->getLocation().x));
    if ( abs(touchBeginPos.x-pTouch->getLocation().x)<290.0 ) {
        returnBgPosition();
        return;
    }
    if ( touchBeginPos.x>pTouch->getLocation().x ) {
        scrollType=INCREMENT;
        if ( listIndex<itemList->count()-1 ) {
            listIndex++;
        } else {
            returnBgPosition();
            return;
        }
    } else {
        scrollType=DECREMENT;
        if ( listIndex>0 ) {
            listIndex--;
        } else {
            returnBgPosition();
            return;
        }
    }
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    bool isContainItem = isContain(item->getItemid(), 3);
    bool isContainAuthor = isContain(item->getAuther(), 1);
    bool isContainCreator = isContain(item->getCreator() , 2);
    
    if ( isContainItem || isContainAuthor || isContainCreator ) {
        animation=CharactorStatus::AnimationStatus_Rotate_left;
    } else {
        animation=CharactorStatus::AnimationStatus_Rotate_right;
    }
    
    //    CCLog("Start Sub Animation On Ciew");
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>( getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    if ( charactor!=NULL ) {
        charactor->addSubAnimation(animation);
    }

    createDetailItem();
}

void ItemDetailLayer::panelFadeOut()
{
    OpacitySprite* opSpriteContent = (OpacitySprite*)getChildByTag(TAG_DETAIL_CONTENT);
    OpacitySprite* opSpriteInfo = (OpacitySprite*)getChildByTag(TAG_DETAIL_INFO);
    bool isContent = opSpriteContent->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    bool isInfo = opSpriteInfo->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    if ( (isContent && isInfo)==false ) {
        return;
    }
    
    opSpriteContent->stopAllActions();
    CCSequence* seq =CCSequence::create(CCDelayTime::create(0.5),CCFadeTo::create(0.3, 0), NULL);
    seq->setTag(TAG_DETAIL_ANIMATION_FADETO);
    opSpriteContent->runAction(seq);
    
    opSpriteInfo->stopAllActions();
        
    seq =CCSequence::create(CCDelayTime::create(0.5),CCFadeTo::create(0.3, 0), NULL);
    seq->setTag(TAG_DETAIL_ANIMATION_FADETO);
    opSpriteInfo->runAction(seq);
    
}

void ItemDetailLayer::panelFadeIn()
{
    OpacitySprite* opSpriteContent = (OpacitySprite*)getChildByTag(TAG_DETAIL_CONTENT);
    OpacitySprite* opSpriteInfo = (OpacitySprite*)getChildByTag(TAG_DETAIL_INFO);
    bool isContent = opSpriteContent->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    bool isInfo = opSpriteInfo->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    if ( (isContent && isInfo)==false ) {
        return;
    }
    
    float delayTime = 0.0;
    opSpriteContent->stopAllActions();
    if ( opSpriteContent->getOpacity()<10.0 ) {
        delayTime=1.5;
    }
    CCSequence* seq = CCSequence::create(CCDelayTime::create(delayTime), CCFadeTo::create(0.3, 255), NULL);
    seq->setTag(TAG_DETAIL_ANIMATION_FADETO);
    opSpriteContent->runAction(seq);

    delayTime=0.0;
    opSpriteInfo->stopAllActions();
    if ( opSpriteInfo->getOpacity()<10.0 ) {
        delayTime=1.5;
    }
    seq = CCSequence::create(CCDelayTime::create(delayTime), CCFadeTo::create(0.3, 255), NULL);
    seq->setTag(TAG_DETAIL_ANIMATION_FADETO);
    opSpriteInfo->runAction(seq);
}

void ItemDetailLayer::panelStopAnimation()
{
    OpacitySprite* opSpriteContent = (OpacitySprite*)getChildByTag(TAG_DETAIL_CONTENT);
    OpacitySprite* opSpriteInfo = (OpacitySprite*)getChildByTag(TAG_DETAIL_INFO);
    bool isContent = opSpriteContent->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    bool isInfo = opSpriteInfo->getActionByTag(TAG_DETAIL_ANIMATION_PANELMANAGE)==NULL;
    if ( (isContent && isInfo)==false ) {
        return;
    }

    opSpriteContent->stopAllActions();
    opSpriteInfo->stopAllActions();

}

void ItemDetailLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    float disX = touchBeginPos.x - pTouch->getLocation().x;
    float disY = touchBeginPos.y - pTouch->getLocation().y;
    if ( abs((int)disX) < abs((int)disY) ) {
        return;
    }
    if ( abs(disX)<=moveOffsetY ) {
        return;
    }

    panelStopAnimation();
    
    //背景アイテムの取得
    CCNode* node = (CCNode*)getChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE);
    if ( node==NULL ) {
        return;
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float defX = touchBeginPos.x-pTouch->getLocation().x-moveOffsetY;
    if ( touchBeginPos.x < pTouch->getLocation().x ) {
        defX = touchBeginPos.x-pTouch->getLocation().x+moveOffsetY;
    }
    CCPoint pos = node->getPosition();
    node->setPosition(ccp( winSize.width/2.0-defX*0.2, pos.y ));
//    CCLOG("%f", node->getPosition().x);

    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_1, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_2, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_3, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_4, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_5, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_6, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_7, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_8, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_9, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_10, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_11, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_12, pTouch);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_13, pTouch);
    
    animDetailItem(TAG_DETAIL_IMAGE, pTouch);
    animDetailItem(TAG_DETAIL_IMAGE_TEMP_LEFT, pTouch);
    animDetailItem(TAG_DETAIL_IMAGE_TEMP_RIGHT, pTouch);

    
}

void ItemDetailLayer::upFlick(cocos2d::CCTouch *pTouch)
{
    float disX = touchBeginPos.x - pTouch->getLocation().x;
    float disY = touchBeginPos.y - pTouch->getLocation().y;
    if ( abs((int)disX) > abs((int)disY) ) {
        returnBgPosition();
        return;
    }
    if ( pTouch->getLocation().y-touchBeginPos.y<250.0 ) {
        returnBgPosition();
        return;
    }


    switch (listType) {
        case PLAN:
        case SEARCH:
            addBookmarkProcess();
            break;
        case BOOKMARK:
            deleteBookmarkProcess();
            break;
        default:
            break;
    }

}

void ItemDetailLayer::addBookmarkProcess()
{
    CCSprite* bg = (CCSprite*)getChildByTag(TAG_DETAIL_IMAGE);
    if ( bg==NULL ) {
        return;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    USERITEM_ADD_RESULT result = UserItemFactory::sharedInstance()->addUserItem((ViewItem*)itemList->objectAtIndex(listIndex));
    if ( result==ALREADY ) {
        ItemViewScene* scene = (ItemViewScene*) getParent();
        scene->createDialog(DialogLayer::DialogType_AlreadyBookMark
                            , NULL, NULL, NULL);
        return;
    }
    if ( result==COUNTMAX ) {
        ItemViewScene* scene = (ItemViewScene*) getParent();
        scene->createDialog(DialogLayer::DialogType_BookMarkCountMax
                            , NULL, NULL, NULL);
        return;
    }
    
    isNaviAnim=false;
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    
    
    if (DateTimeUtil::isBefore(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
        addBookmarkCalender();
    }
    
    removeAllChildParts(false);
   
    bg->runAction(
                  CCSequence::create(
                                     CCMoveTo::create(0.5, ccp(winSize.width/2.0, winSize.height*2.0)), CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::addBookMarkCallback)), NULL));
}

void ItemDetailLayer::addBookmarkCalender()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    //プライバシーの設定に関しての初期化含み。
    CalenderManager::checkPrivacy();
    //カレンダー追加
    bool result = CalenderManager::addCalender( item->getTitle()->getCString(), item->getReleaseDate()->getCString() );
    
    ItemViewScene* scene = (ItemViewScene*) getParent();
    
    if ( result ) {
        scene->createSerif("カレンダーに発売予定を登録しました！。", true);
    } else {
        scene->createSerif("カレンダーにアイテムを登録しませんでした。登録したい場合は設定>プライバシー>カレンダーよりこのアプリのアクセスを許可してください。", true);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#endif
}

void ItemDetailLayer::addBookMarkCallback()
{
    isNaviChange=false;
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_AddBookMark
                        , this, callfunc_selector(ItemDetailLayer::createDetailItem), NULL);
    
}

void ItemDetailLayer::deleteBookmarkProcess()
{
    CCSprite* bg = (CCSprite*)getChildByTag(TAG_DETAIL_IMAGE);
    if ( bg==NULL ) {
        return;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    UserItemFactory::sharedInstance()->deleteUserItem((ViewItem*)itemList->objectAtIndex(listIndex));
    //画面に戻ったときに表示するアイテムを設定する。
    setItemList(UserItemFactory::sharedInstance()->getUserItemList());
    if ( listIndex==0 ) {
        listIndex=0;
    } else if (itemList->count()==0) {
        listIndex=0;
    } else if ( listIndex>=itemList->count() ) {
        listIndex=itemList->count()-1;
    }
    isNaviAnim=false;
    
    CCCallFunc* func = NULL;
    if ( itemList->count()==0 ) {
        func = CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::deleteBookMarkWithNoItemCallback));
    } else {
        func = CCCallFunc::create(this, callfunc_selector(ItemDetailLayer::deleteBookMarkCallback));
    }

    removeAllChildParts(true);
    
    bg->runAction(
                  CCSequence::create(
                                     CCMoveTo::create(0.5, ccp(winSize.width/2.0, winSize.height*2.0)), func, NULL));
}

void ItemDetailLayer::deleteBookMarkCallback()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_DeleteBookMark
                        , this, callfunc_selector(ItemDetailLayer::createDetailItem), NULL);    
}

void ItemDetailLayer::deleteBookMarkWithNoItemCallback()
{
    ItemViewScene* scene = (ItemViewScene*) getParent();
    scene->createDialog(DialogLayer::DialogType_DeleteBookMarkWithoutRemove
                        , this, callfunc_selector(ItemDetailLayer::returnList), NULL);
}


void ItemDetailLayer::update(float delta)
{
    if ( !isNaviAnim ) return;
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_1, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_2, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_3, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_4, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_5, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_6, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_7, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_8, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_9, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_10, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_11, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_12, NULL);
    animNaviItem(TAG_DETAIL_ITEMNAVI_ITEM_13, NULL);

    animDetailItem(TAG_DETAIL_IMAGE, NULL);
    animDetailItem(TAG_DETAIL_IMAGE_TEMP_LEFT, NULL);
    animDetailItem(TAG_DETAIL_IMAGE_TEMP_RIGHT, NULL);

}

void ItemDetailLayer::animDetailItem(int tag, cocos2d::CCTouch *pTouch)
{
    int baseTag = TAG_DETAIL_IMAGE_TEMP_LEFT;
    int getIndex = tag-baseTag;
    
    float moveMax = 50.0;
    float deg = 0.0;

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    if ( isNaviAnim ) {
        CCNode* node = (CCNode*)getChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE);
        if ( node==NULL ) {
            return;
        }
        CCPoint pos = node->getPosition();
        
        float defX = winSize.width/2.0-pos.x;
        if (defX>moveMax) defX = moveMax;
        if (defX<-moveMax) defX = -moveMax;
        deg = detailImageDeg[getIndex] - 90.0 * defX/moveMax;
        
    } else {
        moveMax = 250;
        float defX = touchBeginPos.x-pTouch->getLocation().x;
        if (defX>moveMax) defX = moveMax;
        if (defX<-moveMax) defX = -moveMax;
        deg = detailImageDeg[getIndex] - 90.0 * defX/moveMax;
    }
    
    
    CCSprite* sprite = dynamic_cast<CCSprite*>(getChildByTag(tag));
    if ( sprite==NULL ) {
        return;
    }

    
    float asobi = winSize.width - detailCicrleWidth;
    float posX = asobi/2.0 + (detailCicrleWidth/2.0+sin(CC_DEGREES_TO_RADIANS(deg)) * detailCicrleWidth/2.0);
    float posY = adjustY;
    
    sprite->setPosition(ccp(posX, posY));
    
    float scale = (DetailImgSize.width-200 + 200.0 * cos(CC_DEGREES_TO_RADIANS(deg)));
    CCSize size = sprite->getContentSize();
    sprite->setScale(scale/size.width);
    
    if (deg>90) deg=90;
    if (deg<-90) deg=-90;
    
    float opacity = 255.0*cos(CC_DEGREES_TO_RADIANS(deg));
    sprite->setOpacity(opacity);
    if (tag==TAG_DETAIL_IMAGE) animDetailItemTitle(deg);
}

void ItemDetailLayer::animDetailItemTitle(float deg)
{
    //透過の計算
    int currentOpacity = 0;
    int leftOpacity = 0;
    int rightOpacity = 0;
    if ( deg>-20.0 && deg < 20.0 ) {
        currentOpacity=255;
    } else if (deg < -70.0) {
        rightOpacity=255;
    } else if (deg>70.0) {
        leftOpacity = 255;
    } else if (deg>20.0 && deg <=45.0) {
        float delta = deg - 20.0;
        float delataOpacoty = (255.0/25.0) * delta;
        currentOpacity = 255 - delataOpacoty;
    } else if (deg>=45.0 && deg <=70.0) {
        float delta = deg - 45.0;
        float delataOpacoty = (255.0/25.0) * delta;
        leftOpacity = delataOpacoty;
    } else if (deg<=-20.0 && deg >=-45.0) {
        float delta = (deg*-1.0) - 20.0;
        float delataOpacoty = (255.0/25.0) * delta;
        currentOpacity = 255 - delataOpacoty;
    } else if (deg<=-45.0 && deg >=-70.0) {
        float delta = (deg*-1.0) - 45.0;
        float delataOpacoty = (255.0/25.0) * delta;
        rightOpacity = delataOpacoty;
    }
    
    CCSprite* sprite = dynamic_cast<CCSprite*>(getChildByTag(TAG_DETAIL_TITLE));
    if (sprite==NULL) return;
    CCArray* ary = sprite->getChildren();
    if ( ary!=NULL ) {
    for (int i=0; i<ary->count(); i++) {
        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(ary->objectAtIndex(i));
        if (label==NULL) continue;
        if ( label->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
            label->setOpacity(currentOpacity);
        }
        if ( label->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
            label->setOpacity(leftOpacity);
        }
        if ( label->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
            label->setOpacity(rightOpacity);
        }

    }
    }
    
    int currentOpacity2 = 0;
    int leftOpacity2 = 0;
    int rightOpacity2 = 0;
    
    OpacitySprite* opSprite =dynamic_cast<OpacitySprite*>(getChildByTag(TAG_DETAIL_INFO));
    if (opSprite==NULL) return;
    if (currentOpacity > opSprite->getOpacity()) {
        currentOpacity2 = opSprite->getOpacity();
    } else {
        currentOpacity2 = currentOpacity;
    }
    if (rightOpacity > opSprite->getOpacity()) {
        rightOpacity2 = opSprite->getOpacity();
    } else {
        rightOpacity2 = rightOpacity;
    }
    if (leftOpacity > opSprite->getOpacity()) {
        leftOpacity2 = opSprite->getOpacity();
    } else {
        leftOpacity2 = leftOpacity;
    }
    ary = opSprite->getChildren();
    if ( ary!=NULL ) {
    for (int i=0; i<ary->count(); i++) {
        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(ary->objectAtIndex(i));
        if (label!=NULL) {
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                label->setOpacity(currentOpacity2);
            }
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                label->setOpacity(leftOpacity2);
            }
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                label->setOpacity(rightOpacity2);
            }
        }
        CCSprite* sprite = dynamic_cast<CCSprite*>(ary->objectAtIndex(i));
        if (sprite!=NULL) {
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                sprite->setOpacity(currentOpacity2);
            }
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                sprite->setOpacity(leftOpacity2);
            }
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                sprite->setOpacity(rightOpacity2);
            }
        }
        CommonButton* button = dynamic_cast<CommonButton*>(ary->objectAtIndex(i));
        if (button!=NULL) {
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                button->setOpacity(currentOpacity2);
            }
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                button->setOpacity(leftOpacity2);
            }
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                button->setOpacity(rightOpacity2);
            }
        }
    }
    }

    opSprite =dynamic_cast<OpacitySprite*>(getChildByTag(TAG_DETAIL_CONTENT));
    if (opSprite==NULL) return;
    if (currentOpacity > opSprite->getOpacity()) {
        currentOpacity2 = opSprite->getOpacity();
    } else {
        currentOpacity2 = currentOpacity;
    }
    if (rightOpacity > opSprite->getOpacity()) {
        rightOpacity2 = opSprite->getOpacity();
    } else {
        rightOpacity2 = rightOpacity;
    }
    if (leftOpacity > opSprite->getOpacity()) {
        leftOpacity2 = opSprite->getOpacity();
    } else {
        leftOpacity2 = leftOpacity;
    }
    ary = opSprite->getChildren();
    if ( ary!=NULL ) {
    for (int i=0; i<ary->count(); i++) {
        CCLabelTTF* label = dynamic_cast<CCLabelTTF*>(ary->objectAtIndex(i));
        if (label!=NULL) {
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                label->setOpacity(currentOpacity2);
            }
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                label->setOpacity(leftOpacity2);
            }
            if ( label->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                label->setOpacity(rightOpacity2);
            }
        }
        CCSprite* sprite = dynamic_cast<CCSprite*>(ary->objectAtIndex(i));
        if (sprite!=NULL) {
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                sprite->setOpacity(currentOpacity2);
            }
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                sprite->setOpacity(leftOpacity2);
            }
            if ( sprite->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                sprite->setOpacity(rightOpacity2);
            }
        }
        CommonButton* button = dynamic_cast<CommonButton*>(ary->objectAtIndex(i));
        if (button!=NULL) {
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_CENTER ) {
                button->setOpacity(currentOpacity2);
            }
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_LEFT ) {
                button->setOpacity(leftOpacity2);
            }
            if ( button->getTag()==TAG_DETAIL_FADE_TEXT_RIGHT ) {
                button->setOpacity(rightOpacity2);
            }
        }
    }
    }
}

void ItemDetailLayer::animNaviItem(int tag, CCTouch* pTouch)
{
    int baseTag = TAG_DETAIL_ITEMNAVI_ITEM_1;
    int getIndex = tag-baseTag;

    float moveMax = 50.0;
    float deg = 0.0;
    
    if ( isNaviAnim ) {
        CCNode* node = (CCNode*)getChildByTag(TAG_DETAIL_IMAGE_TARGET_NODE);
        if ( node==NULL ) {
            return;
        }
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint pos = node->getPosition();

        float defX = winSize.width/2.0-pos.x;
        if (defX>moveMax) defX = moveMax;
        if (defX<-moveMax) defX = -moveMax;
        deg = baseDeg[getIndex] - 30.0 * defX/moveMax;
        
    } else {
        moveMax = 250;
        float defX = touchBeginPos.x-pTouch->getLocation().x;
        if (defX>moveMax) defX = moveMax;
        if (defX<-moveMax) defX = -moveMax;
        deg = baseDeg[getIndex] - 30.0 * defX/moveMax;
    }
    
    CCSprite* itemNavi = (CCSprite*)getChildByTag(TAG_DETAIL_ITEMNAVI);
    if ( itemNavi==NULL ) {
        return;
    }
    
    CCSprite* sprite = dynamic_cast<CCSprite*>(itemNavi->getChildByTag(tag));
    if ( sprite==NULL ) {
        return;
    }
    CCSize itemNaviSize = itemNavi->getContentSize();
    
    float asobi = itemNaviSize.width - naviWidth;
    float posX = asobi/2.0 + (naviWidth/2.0+sin(CC_DEGREES_TO_RADIANS(deg)) * naviWidth/2.0);
    float posY = itemNaviSize.height/2.0;

    sprite->setPosition(ccp(posX, posY));
    
    float scale = (ItenNaviImgSize3.width + ItenNaviImgSize3.width * cos(CC_DEGREES_TO_RADIANS(deg)));
    CCSize size = sprite->getContentSize();
    sprite->setScaleX(scale/size.width);
    sprite->setScaleY(scale/size.height);
    
}

void ItemDetailLayer::changeNaviAnim()
{
    isNaviAnim = (isNaviAnim) ? false: true;
}


bool ItemDetailLayer::isContain(cocos2d::CCString *value, int checkType)
{
    CCArray* bookmarkList = UserItemFactory::sharedInstance()->getUserItemList();
    //BookMark1なければFalseだわな。
    if ( bookmarkList==NULL || bookmarkList->count()==0 ) {
        return false;
    }
    switch (checkType) {
        case 1:
        {
            //作家
            for (int i=0; i<bookmarkList->count(); i++) {
                ViewItem* item = (ViewItem*)bookmarkList->objectAtIndex(i);
                if ( strcmp(item->getAuther()->getCString(), value->getCString())==0 ) {
                    return true;
                }
            }
            return false;
        }
            break;
        case 2:
        {
            //イラスト
            for (int i=0; i<bookmarkList->count(); i++) {
                ViewItem* item = (ViewItem*)bookmarkList->objectAtIndex(i);
                if ( strcmp(item->getCreator()->getCString(), value->getCString())==0 ) {
                    return true;
                }
            }
        }
            break;
        case 3:
        {
            //タイトル
            for (int i=0; i<bookmarkList->count(); i++) {
                ViewItem* item = (ViewItem*)bookmarkList->objectAtIndex(i);
                if ( strcmp(item->getItemid()->getCString(), value->getCString())==0 ) {
                    return true;
                }
            }
        }
            break;
        default:
            break;
    }
    return false;
}

void ItemDetailLayer::openBookmarkInfo()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(listIndex);
    BookmarkInDetailLayer* layer = BookmarkInDetailLayer::create(item);
    layer->setPosition(ccp(0.0, -winSize.height));
    layer->runAction(CCMoveTo::create(0.5, ccp(0.0,0.0)));
    addChild(layer, zOrder_ItemBookmark_Back, TAG_DETAIL_BOOKMARK_BG);
    
    //キャラクタレイヤーの移動
    
    CharactorLayer* charactorLayer = dynamic_cast<CharactorLayer*>(getParent()->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    
    if ( charactorLayer!=NULL ) {
        CCActionInterval* act1 = CCMoveBy::create(0.5, ccp(0.0, winSize.height));
        CCEaseBackIn* ease1 = CCEaseBackIn::create(act1);
        charactorLayer->runAction(ease1);
    }
    
}

void ItemDetailLayer::removeAllChildParts(bool isNaviItem)
{
    CCNode* node = getChildByTag(TAG_DETAIL_TITLE);
    node->removeAllChildrenWithCleanup(true);
    node = getChildByTag(TAG_DETAIL_INFO);
    node->removeAllChildrenWithCleanup(true);
    node = getChildByTag(TAG_DETAIL_CONTENT);
    node->removeAllChildrenWithCleanup(true);
    if ( isNaviItem ) {
        node = getChildByTag(TAG_DETAIL_ITEMNAVI);
        node->removeAllChildren();
    }

}

/*******************************************************/
/*******************************************************/
/**********          ServerSyncLaye         ************/
/*******************************************************/
/*******************************************************/


ServerSyncLayer::ServerSyncLayer()
{
    startPos = CCPointZero;
    labelIndex=0;
    labelSize = CCSizeMake(500, 30);
    startPos = CCPointZero;
}

ServerSyncLayer::~ServerSyncLayer()
{
    startPos = CCPointZero;
    labelIndex=0;
    labelSize = CCSizeMake(500, 30);
    startPos = CCPointZero;
}

bool ServerSyncLayer::init(CCObject* target, SEL_CallFunc callfunc)
{
    if ( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 128)) ) {
        return false;
    }
    m_pTarget=target;
    m_pCallfunc=callfunc;
    //初期値の計算
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    startPos = ccp(winSize.width/2.0, winSize.height-150);
    
    addProcessMessage(CCString::create("Start Server Sync. It has 4 Processes"));
    addProcessMessage(CCString::create("Connect Server. ( Process 1/4 )"));
    
    addChild(ViewItemAsyncProcesser::create(this, callfunc_selector(ServerSyncLayer::closeSyncLayer)));
    
    
    return true;
}

ServerSyncLayer* ServerSyncLayer::create(CCObject* target, SEL_CallFunc callfunc)
{
    ServerSyncLayer* pRet = new ServerSyncLayer();
    if (pRet && pRet->init(target, callfunc)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
    
}


void ServerSyncLayer::addProcessMessage(cocos2d::CCString *str)
{
    CCLabelTTF* label = NULL;
    //カレントコンソールの削除
    label = dynamic_cast<CCLabelTTF*>(getChildByTag(TAG_SERVERSYNC_CURRENT_CONSOLE));
    if ( label!=NULL ) {
        removeChildByTag(TAG_SERVERSYNC_CURRENT_CONSOLE, true);
    }
    addMessageCommon(str, false);
    labelIndex++;
    addMessageCommon(CCString::create(""), true);

}

void ServerSyncLayer::addMessageCommon(cocos2d::CCString *str, bool isConsole)
{
    CCLabelTTF* label = NULL;
    //TAGの設定
    int tag = TAG_SERVERSYNC_MESSAGE;
    if (isConsole) {
        //カレントコンソールの削除
        label = dynamic_cast<CCLabelTTF*>(getChildByTag(TAG_SERVERSYNC_CURRENT_CONSOLE));
        if ( label!=NULL ) {
            removeChildByTag(TAG_SERVERSYNC_CURRENT_CONSOLE, true);
        }
        tag=TAG_SERVERSYNC_CURRENT_CONSOLE;
    }
    
    //表示位置の計算
    CCPoint point = ccp(startPos.x, startPos.y-labelIndex*labelSize.height);
    
    //表示場所の計算
    CCString* labelStr = CCString::createWithFormat("Sync Process > %s", str->getCString());
    
    label = CCLabelTTF::create(labelStr->getCString(), FONT_W6, 16, labelSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(point);
    label->setColor(ccc3(0, 255, 0));
    addChild(label, 0, tag);
    if ( !isConsole ) return;
    label->runAction(AnimationFactory::sharedInstance()->getActions("icon-inpact"));
}

void ServerSyncLayer::noticeCreate()
{
    
    CCArray* notices = CCArray::create();
    
    CCArray* planList = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    CCArray* bookmarkList = UserItemFactory::sharedInstance()->getUserItemList();
    
    if ( planList==NULL || planList->count()==0 ) {
        return;
    }
    if ( bookmarkList==NULL || bookmarkList->count()==0 ) {
        return;
    }
    
    for (int i=0; i<planList->count(); i++) {
        ViewItem* plan = dynamic_cast<ViewItem*>(planList->objectAtIndex(i));
        if ( plan==NULL ) {
            continue;
        }
        if ( DateTimeUtil::isAfter(DateTimeUtil::getNowDateTime(), plan->getReleaseDate()) ) {
            continue;
        }
        for (int j=0; j<bookmarkList->count(); j++) {
            ViewItem* bookmark = dynamic_cast<ViewItem*>(bookmarkList->objectAtIndex(j));
            if ( strcmp(bookmark->getItemid()->getCString(), plan->getItemid()->getCString())==0 ) {
               
                
                continue;
            }

            if ( bookmark->getAuther()->length()!=0 && strcmp(bookmark->getAuther()->getCString(), plan->getAuther()->getCString())==0 ) {
                CCString* title = CCString::createWithFormat("【作家：%s】\n%s",plan->getAuther()->getCString(), plan->getTitle()->getCString());
                plan->setTitle(title);
                notices->addObject(plan);
                continue;
            }
            if ( bookmark->getCreator()->length()!=0 && strcmp(bookmark->getCreator()->getCString(), plan->getCreator()->getCString())==0 ) {
                CCString* title = CCString::createWithFormat("【イラスト：%s】\n%s",plan->getCreator()->getCString(), plan->getTitle()->getCString());
                plan->setTitle(title);
                notices->addObject(plan);
                continue;
            }
        }
    }
    
    ItemViewScene* scene =  dynamic_cast<ItemViewScene*>(getParent());
    if ( scene==NULL ) {
        return;
    }
    scene->setNoticeItemList(notices);
    
}

void ServerSyncLayer::closeSyncLayer()
{
//    if ( UserDataDefault::sharedInstance()->getUpdateResult() ) {
        noticeCreate();
//    }
    
    if ( ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate()->count() < 50 ) {
        //サイズが５０以下の場合はサーバのデータ更新がうまく行っていない可能性がある。
        ItemViewScene *scene = (ItemViewScene*)getParent();
        scene->createDialog(DialogLayer::DialogType_Bud_Data, m_pTarget, m_pCallfunc, NULL);
        return;
    }
    
    if ( m_pTarget && m_pCallfunc ) {
        (m_pTarget->*m_pCallfunc)();
    }
//    removeFromParentAndCleanup(true);
}

BgLayer::BgLayer()
{
    isClose= false;
}

BgLayer::~BgLayer()
{
    isClose= false;
}

BgLayer* BgLayer::create()
{
    BgLayer* pRet = new BgLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

bool BgLayer::init()
{
    ccColor4B ccc4Nomal = ccc4(0, 0, 0, 160);
    ccColor4B color = ccc4Nomal;
    
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
//    schedule(schedule_selector(BgLayer::createTest), 0.05);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    
    setTouchEnabled(true);

    return true;
}

void BgLayer::createTest(float delta)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* effect = CCSprite::createWithSpriteFrameName("img_detail_effect_part2.png");
//    effect->setPosition(ccp(winSize.width/4.0,winSize.height/2.0));
    effect->setPosition(ccp(0.0,winSize.height));
//    effect->setScaleY(1.0);
//    effect->setRotation(0);
    effect->setAnchorPoint(ccp(0.5,0.3));
    
    CCActionInterval* move = CCMoveTo::create(3.0, ccp(winSize.width-delta,0.0));
    CCActionInterval* rotate = CCRotateBy::create(3.0, 360);
    CCCallFunc* func = CCCallFunc::create(effect, callfunc_selector(CCSprite::removeFromParent));
//    CCCallFunc* func2 = CCCallFunc::create(this, callfunc_selector(BgLayer::createTest2));
    
    CCSpawn* spawn = CCSpawn::create(move, rotate, NULL);
    
    
    CCSequence* seq = CCSequence::create(spawn,  func, NULL);
    
    effect->runAction(seq);
    addChild(effect,4);
}

void BgLayer::createTest2()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* effect = CCSprite::createWithSpriteFrameName("img_detail_effect_part2.png");
    effect->setPosition(ccp(winSize.width/4.0*3.0,winSize.height/2.0));
    effect->setScaleY(0.7);
    effect->setRotation(60);
    effect->setAnchorPoint(ccp(0.5,0.5));
    
    CCActionInterval* move = CCMoveTo::create(1.0, ccp(winSize.width/4.0,winSize.height/2.0));
    CCActionInterval* rotate = CCRotateBy::create(1.0, -120);
    CCCallFunc* func = CCCallFunc::create(effect, callfunc_selector(CCSprite::removeFromParent));
    
    CCSpawn* spawn = CCSpawn::create(move, rotate, NULL);
    
    CCSequence* seq = CCSequence::create(spawn, func, NULL);
    
    effect->runAction(seq
                      );
    addChild(effect,2);

}



void BgLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool BgLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    return true;
}

void BgLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    if ( isClose ) {
        return;
    }
    isClose= true;
    stopAllActions();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = getPosition();
    
    CCActionInterval* fadeto = CCFadeTo::create(0.5, 0);
    CCActionInterval* act1 = CCMoveTo::create(0.5, ccp(pos.x, -winSize.height));
    CCSpawn* spwn = CCSpawn::create(fadeto, act1, NULL);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BgLayer::removeFromParent));
    
    runAction(CCSequence::create(spwn, func, NULL));
    //キャラクターレイヤ
    ItemDetailLayer* layer =dynamic_cast<ItemDetailLayer*>(getParent());
    ItemViewScene* scene = dynamic_cast<ItemViewScene*>(layer->getParent());
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>(scene->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    
    if ( charactor!=NULL ) {
        CCActionInterval* act1 = CCMoveBy::create(1.0, ccp(0.0, -winSize.height));
        CCEaseBackOut* ease1 = CCEaseBackOut::create(act1);
        charactor->runAction(ease1);
    }
    CCObject* child;
    CCARRAY_FOREACH(m_pChildren, child)
    {
        CCActionInterval* fadeto = CCFadeTo::create(0.5, 0);
        CCNode* pNode = (CCNode*)child;
        pNode->runAction(fadeto);
    }

    //    removeFromParentAndCleanup(true);
}

void BgLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}

SettingLayer::SettingLayer()
{
    isClose= true;
    isPush = true;
}

SettingLayer::~SettingLayer()
{
    isClose= true;
    isPush = true;
}

SettingLayer* SettingLayer::create()
{
    SettingLayer* pRet = new SettingLayer();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

bool SettingLayer::init()
{
    ccColor4B ccc4Nomal = ccc4(0, 0, 0, 160);
    ccColor4B color = ccc4Nomal;
    
    if ( !CCLayerColor::initWithColor(color) ) {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    setPosition(ccp(-winSize.width, 0.0));

    CCSprite* sprite = NULL;
    
    sprite = CCSprite::create("SettingNaviBar.png");
    sprite->setPosition(ccp(winSize.width+6.0, winSize.height/2.0));
    addChild(sprite, 0, TAG_SETTINGLAYER_NAVI_BAR);
    
    sprite = CCSprite::create("SettingNaviBar.png");
    sprite->setRotation(180);
    sprite->setPosition(ccp(winSize.width-6.0, winSize.height/2.0+6.0));
    addChild(sprite, 0, TAG_SETTINGLAYER_NAVI_BAR);
    
//    initLayout();
    
    return true;
}

void SettingLayer::initLayout()
{
    this->removeAllChildrenWithCleanup(true);

    
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCLabelTTF* label = NULL;
    CommonButton* button = NULL;
    CCSprite* sprite = NULL;
    
    sprite = CCSprite::create("SettingNaviBar.png");
    sprite->setPosition(ccp(winSize.width+6.0, winSize.height/2.0));
    addChild(sprite, 0, TAG_SETTINGLAYER_NAVI_BAR);
    
    sprite = CCSprite::create("SettingNaviBar.png");
    sprite->setRotation(180);
    sprite->setPosition(ccp(winSize.width-6.0, winSize.height/2.0+6.0));
    addChild(sprite, 0, TAG_SETTINGLAYER_NAVI_BAR);
    
    label = CCLabelTTF::create("設定", FONT_W6, 36, CCSizeMake(500, 65), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+400));
    label->setColor(ccc3(255, 255, 0));
    label->setOpacity(0);
    label->runAction(fadeInAction(255));
    addChild(label);

    label = CCLabelTTF::create("１．キーワードによる通知を受信しますか？", FONT_W6, 28, CCSizeMake(600, 65), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+300));
    label->setColor(ccc3(255, 255, 0));
    label->setOpacity(0);
    label->runAction(fadeInAction(255));
    addChild(label);
    
    //受信《はい》の設置
    if ( isPush ) {
        sprite=CCSprite::createWithSpriteFrameName("btn_common_yes.png");
        sprite->setPosition(ccp( winSize.width/2.0-70, winSize.height/2.0 +230 ));
        sprite->setOpacity(0);
        sprite->runAction(fadeInAction(255));
        addChild(sprite);

    } else {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_yes.png"), CCSizeZero, this, callfuncO_selector(SettingLayer::switchPushSetting));
        button->setPosition(ccp( winSize.width/2.0-70, winSize.height/2.0 +230 ));
        button->setOpacity(0);
        button->runAction(fadeInAction(128));
        button->setTouchPriority(0);
        addChild(button);
    }
    
    //受信《いいえ》の設置
    if ( isPush ) {
        button = CommonButton::create(CCScale9Sprite::createWithSpriteFrameName("btn_common_no.png"), CCSizeZero, this, callfuncO_selector(SettingLayer::switchPushSetting));
        button->setPosition(ccp( winSize.width/2.0+70, winSize.height/2.0 +230 ));
        button->setOpacity(0);
        button->runAction(fadeInAction(128));
        addChild(button);
    } else {
        sprite=CCSprite::createWithSpriteFrameName("btn_common_no.png");
        sprite->setPosition(ccp( winSize.width/2.0+70, winSize.height/2.0 +230 ));
        sprite->setOpacity(0);
        sprite->runAction(fadeInAction(255));
        addChild(sprite);
    }
    
    //EditBox Input
    CCSize editBoxSize = CCSizeMake(400.0,50.0);
    CCEditBox* editBox = CCEditBox::create(editBoxSize, CCScale9Sprite::createWithSpriteFrameName("btn_search_keyword.png"));
    editBox->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+20.0));
    editBox->setFontColor(ccYELLOW);
    editBox->setPlaceholderFontSize(3);
    editBox->setMaxLength(10);
//    editBox->setPlaceHolder("　最大１０文字まで");
    editBox->setTouchPriority(0);
    editBox->setOpacity(0);
    editBox->runAction(fadeInAction(255));

    addChild(editBox);
    
    label = CCLabelTTF::create("２．通知キーワードの登録", FONT_W6, 28, CCSizeMake(600, 65), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+150));
    label->setColor(ccc3(255, 255, 0));
    label->setOpacity(0);
    label->runAction(fadeInAction(255));
    addChild(label);

    label = CCLabelTTF::create("キーワードを登録するとデータ更新時に通知が届きます。（iOS７以上）", FONT_W6, 22, CCSizeMake(500, 65), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, winSize.height/2.0+100));
    label->setColor(ccc3(255, 255, 0));
    label->setOpacity(0);
    label->runAction(fadeInAction(255));
    addChild(label);
    
    label = CCLabelTTF::create("※最大１０個、１つにつき１０文字まで", FONT_W6, 18, CCSizeMake(400, 65), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, winSize.height/2.0-20.0));
    label->setColor(ccc3(255, 255, 255));
    label->setOpacity(0);
    label->runAction(fadeInAction(255));
    addChild(label);

}

CCActionInterval* SettingLayer::fadeInAction(int opacity)
{
    CCDelayTime* delay= CCDelayTime::create(0.2);
    CCActionInterval* act = CCSequence::create(delay, CCFadeTo::create(0.3, opacity), NULL);
    return act;
}

void SettingLayer::switchPushSetting()
{
    if (isPush) {
        isPush = false;
    } else {
        isPush = true;
    }
    initLayout();
}

void SettingLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
}

bool SettingLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    touchStart = pTouch->getLocation();
    return true;
}

void SettingLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    
}


void SettingLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    float endPointX = pTouch->getLocation().x;
    float deltaX = endPointX - touchStart.x;
    
    if ( fabs(deltaX)<100 ) {
        return;
    }
    
    isClose=true;
    setTouchEnabled(false);
    doFadeout();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCEaseIn* act = CCEaseIn::create(CCMoveTo::create(0.3, ccp(-winSize.width,0.0)),1.0);
    runAction(act);
}

void SettingLayer::doFadeout()
{
    CCArray* childs = getChildren();
    if (childs==NULL) return;
    for (int i=0; i<childs->count(); i++) {
        CCNode* object = (CCNode*)childs->objectAtIndex(i);
        if (object->getTag()==TAG_SETTINGLAYER_NAVI_BAR) continue;
        object->runAction(CCFadeOut::create(0.1));
    }
}

void SettingLayer::doFadein()
{
    CCArray* childs = getChildren();
    for (int i=0; i<childs->count(); i++) {
        CCNode* object = (CCNode*)childs->objectAtIndex(i);
        CCDelayTime* delay= CCDelayTime::create(0.2);
        CCActionInterval* act = CCSequence::create(delay, CCFadeIn::create(0.3), NULL);
        object->runAction(act);
    }
}

BookmarkInDetailLayer::BookmarkInDetailLayer()
{
    authors=NULL;
    creators=NULL;
    isClose=false;
}

BookmarkInDetailLayer::~BookmarkInDetailLayer()
{
    CC_SAFE_RELEASE_NULL(authors);
    CC_SAFE_RELEASE_NULL(creators);
    isClose=false;
}

BookmarkInDetailLayer* BookmarkInDetailLayer::create(ViewItem* item)
{
    BookmarkInDetailLayer* pRet = new BookmarkInDetailLayer();
    if (pRet && pRet->init(item)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

bool BookmarkInDetailLayer::init(ViewItem* item)
{
    ccColor4B ccc4Nomal = ccc4(0, 0, 0, 160);
    ccColor4B color = ccc4Nomal;
    
    if ( !CCLayerColor::initWithColor(ccc4Nomal) ) {
        return false;
    }

    
    setAuthors(CCArray::create());
    setCreators(CCArray::create());
    
    CCArray* bookmarks = UserItemFactory::sharedInstance()->getUserItemListOrderbyRegdt();
    
    if ( bookmarks==NULL || bookmarks->count()==0 ) {
        return true;
    }
    
    setColor(ccc3(0, 0, 0));
    
    
    //    schedule(schedule_selector(BgLayer::createTest), 0.05);
    if ( item->getAuther()!=NULL && item->getAuther()->length()!=0 ) {
        for (int i=0; i<bookmarks->count(); i++) {
            ViewItem* bookmark = (ViewItem*)bookmarks->objectAtIndex(i);
            if ( bookmark->getAuther()==NULL || bookmark->getAuther()->length()==0 ) {
                continue;
            }
            if ( strcmp(bookmark->getAuther()->getCString(), item->getAuther()->getCString())==0 ) {
                authors->addObject(bookmark);
            }
        }
    }

    if ( item->getCreator()!=NULL && item->getCreator()->length()!=0 ) {
        for (int i=0; i<bookmarks->count(); i++) {
            ViewItem* bookmark = (ViewItem*)bookmarks->objectAtIndex(i);
            if ( bookmark->getCreator()==NULL || bookmark->getCreator()->length()==0 ) {
                continue;
            }
            if ( strcmp(bookmark->getCreator()->getCString(), item->getCreator()->getCString())==0 ) {
                creators->addObject(bookmark);
            }
        }
    }
    
    createContent(item);
    
    setTouchEnabled(true);
    
    return true;
}

void BookmarkInDetailLayer::createContent(ViewItem* item)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = NULL;
    CCScale9Sprite* sprite = NULL;
    bool isAuthor  = authors->count()!=0 ? true: false;
    bool isCreator = creators->count()!=0 ? true: false;

    float basePosY = winSize.height/2.0 + 380.0;
    
    CCSprite* sprite2 = CCSprite::create("frameback.png");
    sprite2->setPosition(ccp(winSize.width/2.0, winSize.height/2.0));
    sprite2->setOpacity(0);
    sprite2->runAction(CCSequence::create(CCDelayTime::create(0.5), CCFadeIn::create(0.5), NULL));

    addChild(sprite2);

    
    //レイヤー名
    label = CCLabelTTF::create("ブックマーク情報の確認", FONT_W6, 32, CCSizeMake(500, 65), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(winSize.width/2.0, basePosY));
    label->setColor(ccc3(255, 255, 0));
    label->setDimensions(CCSizeMake(500.0, 0.0));
    label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
    label->setOpacity(0);
    label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));

    addChild(label);

    
    basePosY = basePosY - label->getContentSize().height/2.0;

    
    if ( isAuthor ) {
        //タイトル（作家）
        CCString* title = CCString::createWithFormat("『%s』（作家）", item->getAuther()->getCString());
        label = CCLabelTTF::create(title->getCString(), FONT_W6, 28, CCSizeMake(500, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        label->setColor(ccc3(255, 0, 255));

        label->setDimensions(CCSize(500.0, 0.0));
        label->setDimensions(CCSize(label->getContentSize().width, label->getContentSize().height+5.0));
        
        label->setOpacity(0);
        label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
        
        basePosY = basePosY - label->getContentSize().height/2.0 - 40.0;
        label->setPosition(ccp(winSize.width/2.0, basePosY));
        basePosY = basePosY - label->getContentSize().height/2.0;
        
        
        addChild(label);
        
        float startPosY = basePosY;
        
        for (int i=0; i<authors->count(); i++) {
            ViewItem* item = (ViewItem*)authors->objectAtIndex(i);
            label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 24, CCSizeMake(400, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);

            label->setDimensions(CCSizeMake(400.0, 0.0));
            label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
 
            label->setOpacity(0);
            label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
            
            basePosY = basePosY - label->getContentSize().height/2.0 - 20.0;
            label->setPosition(ccp(winSize.width/2.0, basePosY));
            basePosY = basePosY - label->getContentSize().height/2.0;
            //            label->setColor(ccc3(0, 0, 0));
            addChild(label, 1);
        }
//        sprite = CCScale9Sprite::createWithSpriteFrameName("bg_detail_bookmark_author.png");
//        sprite->setContentSize(CCSizeMake(420, startPosY-basePosY + 20));
//        sprite->setPosition(ccp(winSize.width/2.0, startPosY - (startPosY-basePosY)/2.0-10));
//        addChild(sprite, 0);
    }
    if ( isCreator ) {
        //タイトル（作家）
        CCString* title = CCString::createWithFormat("『%s』（イラスト）", item->getCreator()->getCString());
        label = CCLabelTTF::create(title->getCString(), FONT_W6, 28, CCSizeMake(500, 80), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        label->setColor(ccc3(0, 255, 0));
        
        label->setDimensions(CCSize(500.0, 0.0));
        label->setDimensions(CCSize(label->getContentSize().width, label->getContentSize().height+5.0));
        label->setOpacity(0);
        label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
        
        basePosY = basePosY - label->getContentSize().height/2.0 - 40.0;
        label->setPosition(ccp(winSize.width/2.0, basePosY));
        basePosY = basePosY - label->getContentSize().height/2.0;
        
        addChild(label);
        
        float startPosY = basePosY;
        
        for (int i=0; i<creators->count(); i++) {
            ViewItem* item = (ViewItem*)creators->objectAtIndex(i);
            label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 24, CCSizeMake(400, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
            
            label->setDimensions(CCSizeMake(400.0, 0.0));
            label->setDimensions(CCSizeMake(label->getContentSize().width, label->getContentSize().height));
            label->setOpacity(0);
            label->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
            
            basePosY = basePosY - label->getContentSize().height/2.0 - 20.0;
            label->setPosition(ccp(winSize.width/2.0, basePosY));
            basePosY = basePosY - label->getContentSize().height/2.0;
            //            label->setColor(ccc3(0, 0, 0));
            addChild(label, 1);
        }
        
//        sprite = CCScale9Sprite::createWithSpriteFrameName("bg_detail_bookmark_creator.png");
//        sprite->setContentSize(CCSizeMake(420, startPosY-basePosY + 20));
//        sprite->setPosition(ccp(winSize.width/2.0, startPosY - (startPosY-basePosY)/2.0-10));
//        addChild(sprite, 0);
    }
    
    Charactor* charactor = Charactor::create("Charactor_h_350_shadow.png");
    charactor->setPosition(ccp(winSize.width/2.0+100, -winSize.height*1.5));
    charactor->setOpacity(0);
    charactor->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    
    addChild(charactor);
    
    SubCharactor* subCharactor = SubCharactor::create(ccp(160,210));
    subCharactor->setOpacity(0);
    subCharactor->runAction(CCSequence::create(CCDelayTime::create(1.0), CCFadeIn::create(0.5), NULL));
    subCharactor->deleteNotification();
    
    charactor->addChild(subCharactor);
    
    sprite2 = CCSprite::createWithSpriteFrameName("img_serif_detail_tap_close.png");
    sprite2->setPosition(ccp(80, 300));
    CCSequence* seq2 = CCSequence::create(CCDelayTime::create(1.0), CCEaseBackOut::create(CCMoveTo::create(1.0, ccp(winSize.width/2.0+100, winSize.height/2.0-300))), NULL);
    charactor->runAction(seq2);
    CCDelayTime* delay = CCDelayTime::create(0.5);
    CCDelayTime* delay2 = CCDelayTime::create(0.5);
    CCFadeOut* fadeout = CCFadeOut::create(0.5);
    CCFadeIn* fadein = CCFadeIn::create(0.0);
    sprite2->setOpacity(0);
    CCSequence* seq = CCSequence::create(delay, fadeout, delay2, fadein, NULL);
    sprite2->runAction(CCRepeatForever::create(seq));
    charactor->addChild(sprite2);
    
}

void BookmarkInDetailLayer::registerWithTouchDispatcher()
{
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool BookmarkInDetailLayer::ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    return true;
}

void BookmarkInDetailLayer::ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
    if ( isClose==true ) {
        return;
    }
    isClose=true;
    
    stopAllActions();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pos = getPosition();
    
    
    CCActionInterval* fadeto = CCFadeTo::create(0.5, 0);
    CCActionInterval* act1 = CCMoveTo::create(0.5, ccp(pos.x, -winSize.height));
    CCSpawn* spwn = CCSpawn::create(fadeto, act1, NULL);
    CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(BgLayer::removeFromParent));
    
    //キャラクターレイヤ
    ItemDetailLayer* layer =dynamic_cast<ItemDetailLayer*>(getParent());
    ItemViewScene* scene = dynamic_cast<ItemViewScene*>(layer->getParent());
    CharactorLayer* charactor = dynamic_cast<CharactorLayer*>(scene->getChildByTag(TAG_ITEMVIEW_CHARACTOR));
    
    if ( charactor!=NULL ) {
        CCActionInterval* act1 = CCMoveBy::create(1.0, ccp(0.0, -winSize.height));
        CCEaseBackOut* ease1 = CCEaseBackOut::create(act1);
        charactor->runAction(ease1);
    }
    
    CCObject* child;
    CCARRAY_FOREACH(m_pChildren, child)
    {
        CCActionInterval* fadeto = CCFadeTo::create(0.5, 0);
        CCNode* pNode = (CCNode*)child;
        pNode->runAction(fadeto);
    }

    
    runAction(CCSequence::create(spwn, func, NULL));
    //    removeFromParentAndCleanup(true);
}

void BookmarkInDetailLayer::ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent)
{
}


