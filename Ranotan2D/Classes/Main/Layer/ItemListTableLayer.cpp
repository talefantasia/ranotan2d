//
//  ItemListLayer
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/18.
//
//

#include "ItemListTableLayer.h"
#include "ViewItemFactory.h"
#include "CommonDefine.h"
#include "DateTimeUtil.h"
#include "ViewItemFactory.h"
#include "UserItemFactory.h"
#include "CreatorsDataFactory.h"

const CCSize ListTableSize = CCSizeMake(600, 678);
const CCSize ListTableSizeBookmark = CCSizeMake(600, 690);
const CCSize ListCellSize = CCSizeMake(540.0, 182.0);
const CCSize ListImgSize = CCSizeMake(117.0, 145.0);
const CCSize ListBgImgSize = CCSizeMake(550, 800);


ItemListTableLayer::ItemListTableLayer()
{
    itemList=NULL;
    table=NULL;
    listType=PLAN;
    planType=PLAN_ALL;
    listIndex=0;
    bookmarkList=NULL;
}

ItemListTableLayer::~ItemListTableLayer()
{
    CC_SAFE_RELEASE(itemList);
    CC_SAFE_RELEASE(table);
    listType=PLAN;
    planType=PLAN_ALL;
    listIndex=0;
    bookmarkList=NULL;
    CCLOG("%s", "~ItemListTableLayer");
}

ItemListTableLayer* ItemListTableLayer::create(int index, ListType type, PlanType plan)
{
    ItemListTableLayer* pRet = new ItemListTableLayer();
    if (pRet && pRet->init(index, type, plan)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

bool ItemListTableLayer::init(int index, ListType type, PlanType plan)
{
    if ( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)) ) {
        return false;
    }
    planType=plan;
    listType=type;
    listIndex=index;
    //データの取得
    
    
    initData();
    createTable();

    CCLOG("Create List %d", type);
    CCLOG("ListSize %d", itemList->count());

    
    this->setVisible(true);
//    this->setTouchEnabled(false);
    this->schedule(schedule_selector(ItemListTableLayer::update), 5.0);
    
    return true;
}

void ItemListTableLayer::initData()
{
    setBookmarkList(UserItemFactory::sharedInstance()->getUserItemList());
    if ( listType==PLAN ) {
        setItemList(ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate());
        if ( planType==PLAN_BOOKMARK ) {
            setItemList(CreatorsDataFactory::sharedInstance()->getCheckList());
        }
    } else if ( listType==SEARCH ) {
        setItemList(ViewItemFactory::sharedInstance()->createSearchList());
    } else {
        setItemList(UserItemFactory::sharedInstance()->getUserItemListOrderbyRegdt());
    }
    itemCnt=itemList->count();
}

void ItemListTableLayer::createTable()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    if ( listType==BOOKMARK ) {
        setTable(CCTableView::create(this, ListTableSizeBookmark));
    } else {
        setTable(CCTableView::create(this, ListTableSize));
    }
    
//    table->setTouchEnabled(false);
    table->setTouchPriority(1);
    table->setDirection(kCCScrollViewDirectionVertical);
    table->setVerticalFillOrder(kCCTableViewFillTopDown);
    table->setAnchorPoint(ccp(0.5,0));
//    table->setPosition(ccp(winSize.width/2.0-270, winSize.height/2.0-348));
    table->setPosition(ccp(winSize.width/2.0-210, winSize.height/2.0-348));
    table->setDelegate(this);
    addChild(table, 0);
    table->reloadData();
//    if ( listIndex==0 ) {
//        return;
//    }
    table->setContentOffsetInDuration(mathOffsetFromForcusItem(listIndex), 0.5);

    if ( itemCnt>0 ) {
        return;
    }
    CCScale9Sprite* bg = CCScale9Sprite::createWithSpriteFrameName("bg_common_dialog_plan.png");
    bg->setContentSize(CCSizeMake(470, 670));
    bg->setPosition(ccp(winSize.width/2.0+60, winSize.height/2.0-10.0));
    addChild(bg, 0);
    
    CCString* labelStr = CCString::create("ブックマークに登録がありません。\n\n登録すれば発売日に通知など\n\nいろいろ演出が増えますよ！");
    
    if ( listType==SEARCH ) {
        labelStr = CCString::create("検索結果はありませんでした。\n\n検索条件を変更して\n\nもう一度お試しください。");
    } else if ( listType==PLAN && planType==PLAN_BOOKMARK ) {
        labelStr = CCString::create("チェックリストがありませんネ。。\n\nチェックリストは発売予定と\n\nブックマークを比較してピックアップ！\n\n気になる作品はLet's ブックマーク！！");
    }
    
    CCLabelTTF* label = CCLabelTTF::create(labelStr->getCString(), FONT_W6, 24, CCSizeMake(470, 570), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(235, 335));
    label->setColor(ccc3(255, 255, 0));
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    bg->addChild(label);
    
}

CCPoint ItemListTableLayer::mathOffsetFromForcusItem(int index)
{
    CCSize contentSize = table->getContentSize();
    float maxOffset = contentSize.height - ListTableSize.height;
    if ( listType==BOOKMARK ) {
        maxOffset = contentSize.height - ListTableSizeBookmark.height;
    }
    if ( index==-1 ) {
        return CCPointMake(0.0, -maxOffset);
    }
    if ( index<2 ) {
        return CCPointMake(0.0, -maxOffset);
    }
    float nowOffset = -contentSize.height + (ListCellSize.height * index+ListTableSize.height/2.0 + ListCellSize.height/2.0);
    
    if ( nowOffset>0.0 ) {
        nowOffset=0.0;
    }
    CCLOG("nowOffset is %f", nowOffset);
    
    return CCPointMake(0.0, nowOffset);
    
    
    
}


CCSize ItemListTableLayer::cellSizeForTable(cocos2d::extension::CCTableView *table)
{
    return ListCellSize;
}

unsigned int ItemListTableLayer::numberOfCellsInTableView(cocos2d::extension::CCTableView *table)
{
    return itemCnt;
}

CCTableViewCell* ItemListTableLayer::tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx)
{
    CCTableViewCell* cell = table->dequeueCell();
    if ( !cell ) {
        cell = new CCTableViewCell();
        cell->autorelease();
    } else {
        //こどもは全て削除
        cell->removeAllChildren();
    }
    
    CCSprite* sprite = NULL;
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(idx);
    
    cell->setTag(idx);
    
    //背景
    float posX = ListCellSize.width/2.0;
    float posY = ListCellSize.height/2.0;

    bool isTitle = isContain(item->getItemid(), 3);
    bool isAuthor = isContain(item->getAuther(), 1);
    bool isCreator = isContain(item->getCreator(), 2);

    if ( isTitle && listType!=BOOKMARK ) {
        sprite = CCSprite::createWithSpriteFrameName("bg_list_item_bookmark.png");
        CCSprite* effect = CCSprite::createWithSpriteFrameName("bg_list_item_bookmark_effect.png");
        effect->setPosition(ccp(posX, posY));
        cell->addChild(effect);
        
        CCFadeIn* fadein1 = CCFadeIn::create(1.0);
        CCDelayTime* delay = CCDelayTime::create(0.3);
        CCFadeOut* fadeout1 = CCFadeOut::create(1.0);
        CCSequence* seq1 = CCSequence::create(fadein1, delay, fadeout1, NULL);
        effect->runAction(CCRepeatForever::create(seq1));
    } else if ((isAuthor || isCreator) && listType!=BOOKMARK )  {
        CCSprite* effect = CCSprite::createWithSpriteFrameName("bg_list_item_check_effect.png");
        effect->setPosition(ccp(posX, posY));
        cell->addChild(effect);
        
        CCFadeIn* fadein1 = CCFadeIn::create(1.0);
        CCDelayTime* delay = CCDelayTime::create(0.3);
        CCFadeOut* fadeout1 = CCFadeOut::create(1.0);
        CCSequence* seq1 = CCSequence::create(fadein1, delay, fadeout1, NULL);
        effect->runAction(CCRepeatForever::create(seq1));
        
        sprite = CCSprite::createWithSpriteFrameName("bg_list_item_check.png");
    } else {
        sprite = CCSprite::createWithSpriteFrameName("bg_list_item.png");
    }
    sprite->setPosition(ccp(posX, posY));
    //    sprite->setOpacity(64);
    cell->addChild(sprite);
    
    
    CCImage *image = new CCImage();
    std::string imagePath =UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "S_" +item->getItemid()->getCString();
//    if ( listType==SEARCH ) {
//        imagePath =UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "search_S_" +item->getItemid()->getCString();
//    }
    bool flg = image->initWithImageFile(imagePath.c_str());
    
    image->autorelease();
    // 3. CCTexture2DからCCSpriteへ
    
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithImage(image);
    texture->autorelease();
    if ( flg ) {
        sprite = CCSprite::createWithTexture(texture);
    } else {
        sprite = CCSprite::create();
    }
    sprite -> setPosition(ccp(107, posY-2));
    CCSize contentSize = sprite->getContentSize();
    float scaleX = ListImgSize.width / contentSize.width;
    float scaleY = ListImgSize.height / contentSize.height;
    sprite -> setScaleX(scaleX);
    sprite -> setScaleY(scaleY);
    cell->addChild(sprite);
    
    
    sprite = CCSprite::createWithSpriteFrameName("bg_list_item_title.png");
    sprite->setPosition(ccp(ListCellSize.width/2.0+70, posY+43));
    cell->addChild(sprite);
    
    
    CCLabelTTF* label = NULL;
    label = CCLabelTTF::create(item->getTitle()->getCString(), FONT_W6, 20, CCSizeMake(ListCellSize.width-260, 50), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(ListCellSize.width/2.0+70, posY+43));
    if ( isTitle ) {
        label->setColor(ccc3(255, 255, 0));
    }
    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    cell->addChild(label);
    
    sprite = CCSprite::createWithSpriteFrameName("bg_list_item_info.png");
    sprite->setPosition(ccp(ListCellSize.width/2.0+70, posY-36));
    cell->addChild(sprite);
    
    CCString* str = CCString::createWithFormat("作家　　：%s", item->getAuther()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 16, CCSizeMake(ListCellSize.width-260, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(51, 51, 51));
    if ( !isTitle && isAuthor ) {
        label->setColor(ccc3(0, 165, 0));
    }
    label->setPosition(ccp(ListCellSize.width/2.0+75, posY-10));
    //    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    cell->addChild(label);
    
    str = CCString::createWithFormat("イラスト：%s", item->getCreator()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 16, CCSizeMake(ListCellSize.width-260, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(51, 51, 51));
    if ( !isTitle && isCreator ) {
        label->setColor(ccc3(0, 165, 0));
    }
    label->setPosition(ccp(ListCellSize.width/2.0+75, posY-29));
    //    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    cell->addChild(label);
    
    str = CCString::createWithFormat("レーベル：%s", item->getLabel()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 16, CCSizeMake(ListCellSize.width-260, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(32, 32, 32));
    label->setPosition(ccp(ListCellSize.width/2.0+75, posY-47));
    //    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    cell->addChild(label);
    
    str = CCString::createWithFormat("発売日　：%s", item->getReleaseDate()->getCString());
    label = CCLabelTTF::create(str->getCString(), FONT_W6, 16, CCSizeMake(ListCellSize.width-260, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    label->setColor(ccc3(51, 51, 51));
    label->setPosition(ccp(ListCellSize.width/2.0+75, posY-65));
    //    label->enableShadow(CCSizeMake(2, -2), 0.5f, 0.8f, true);
    cell->addChild(label);
    
    if ( !DateTimeUtil::isBefore(DateTimeUtil::getNowDateTime(), item->getReleaseDate()) ) {
        CCSprite* nowSale = CCSprite::createWithSpriteFrameName("icon_list_nowsale.png");
        nowSale->setPosition(ccp(150, ListCellSize.height-20));
        nowSale->setRotation(30);
        cell->addChild(nowSale);
        
    }
    
    return cell;
}

void ItemListTableLayer::tableCellTouched(CCTableView *table, CCTableViewCell *cell)
{
    ItemListLayer* layer = (ItemListLayer*) getParent();
    layer->changeDetailReady(cell->getTag());
}

void ItemListTableLayer::scrollViewDidScroll(CCScrollView* view){
}

void ItemListTableLayer::scrollViewDidZoom(CCScrollView* view)
{
}


void ItemListTableLayer::update(float delta)
{
    //表が動いているかのチェック
    //    checkTableMove();
    //フォーカスを当てるアイテムの設定
    mathFoucsItemFromOffset();
    CCLOG("UpdateFlag is %s", UserDataDefault::sharedInstance()->isUpdate() ? "YES": "NO");
}
//
//void ItemListLayer::checkTableMove()
//{
//    isMoveTableBefore=isMoveTable;
//    CCPoint offsetPoint = table->getContentOffset();
//    isMoveTable = (offsetPoint.y!=beforeOffsetY) ? true: false;
//    scrollType = ( beforeOffsetY>offsetPoint.y ) ? DECREMENT :INCREMENT;
//    
//    beforeOffsetY = offsetPoint.y;
//}
//
void ItemListTableLayer::mathFoucsItemFromOffset()
{
    CCSize contentSize = table->getContentSize();
    CCPoint offsetPoint = table->getContentOffset();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
 
    
    
    float maxOffset = contentSize.height - winSize.height;
    int tempForcsItem = (itemCnt-1) + (int)((itemCnt-1)*(offsetPoint.y/maxOffset));

    CCLog("全Offset : %f, Current : %f", maxOffset, offsetPoint.y);
 
    //offsetの値によるフォーカスの補正
    if (tempForcsItem<0) tempForcsItem = -1;
    if (tempForcsItem>itemCnt-1) tempForcsItem = -1;
    
//    //フォーカスが変わったか
//    isChangeForcs = (tempForcsItem!=forcsItem) ? true: false;
//    forcsItem=tempForcsItem;
}

bool ItemListTableLayer::isContain(cocos2d::CCString *value, int checkType)
{
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
                if ( item->getCreator()->length()!=0 && strcmp(item->getCreator()->getCString(), value->getCString())==0 ) {
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

