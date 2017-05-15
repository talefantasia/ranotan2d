//
//  UserItemFactory.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/13.
//
//

#include "UserItemFactory.h"
#include "UserDataDefault.h"
#include "CommonDefine.h"

#define KEY_NAME_USERITEM_LIST "KEY_USERITEM_LIST"
#define KEY_NAME_AUTHOR_LIST "KEY_AUTHOR_LIST"
#define KEY_NAME_CREATOR_LIST "KEY_CREATOR_LIST"

#define KEY_NAME_USERITEM_DIC_ITEMID "Itemid"
#define KEY_NAME_USERITEM_DIC_TITLE "Title"
#define KEY_NAME_USERITEM_DIC_AUTHER "Auther"
#define KEY_NAME_USERITEM_DIC_CREATOR "Creator"
#define KEY_NAME_USERITEM_DIC_LABEL "Label"
#define KEY_NAME_USERITEM_DIC_RELEASEDATE "ReleaseDate"
#define KEY_NAME_USERITEM_DIC_PRICE "Price"
#define KEY_NAME_USERITEM_DIC_CONTENTS "Contents"
#define KEY_NAME_USERITEM_DIC_IMGLARGE "ImgLarge"
#define KEY_NAME_USERITEM_DIC_IMGSMALL "ImgSmall"
#define KEY_NAME_USERITEM_DIC_SHOPURL "rakutenPageUrl1"

UserItemFactory* userItemFactory = NULL;

UserItemFactory::UserItemFactory()
{
    mainDic = NULL;
    list=NULL;
}

UserItemFactory::~UserItemFactory()
{
    mainDic = NULL;
    CC_SAFE_RELEASE_NULL(list);
}

bool UserItemFactory::init()
{
    //存在するかどうか確認
    CCDictionary* dic = NULL;
    if(CCFileUtils::sharedFileUtils()->isFileExist(UserDataDefault::sharedInstance()->getBookMarkSaveFilePath())){
        dic = CCDictionary::createWithContentsOfFile(UserDataDefault::sharedInstance()->getBookMarkSaveFilePath().c_str());
    }else{
        dic = CCDictionary::create();
    }
    setMainDic(dic);
    setList(createUserItemList());
    
    return true;

}

UserItemFactory* UserItemFactory::sharedInstance()
{
    if (!userItemFactory) {
        userItemFactory = UserItemFactory::create();
        CC_SAFE_RETAIN(userItemFactory);
    }
    return userItemFactory;

}

CCArray* UserItemFactory::getUserItemDicList()
{
    CCArray* ary = (CCArray*)mainDic->objectForKey(KEY_NAME_USERITEM_LIST);
    //無い場合はサイズ０のリストを返す。
    if ( !ary ) {
        CCLOG("UserItemList size is Zero");
        return CCArray::create();
    }
    return ary;
}

CCArray* UserItemFactory::createUserItemList()
{
    CCArray* ary = getUserItemDicList();
    CCArray* result = CCArray::create();
    for (int i=0; i<ary->count(); i++) {
        result->addObject(createItemFromDic((CCDictionary*)ary->objectAtIndex(i)));
    }
    CCLOG("UserItemList Original size is %d", ary->count());
    CCLOG("UserItemList create size is %d", result->count());
    return result;
}

CCArray* UserItemFactory::getUserItemList()
{
    return list;
}

CCArray* UserItemFactory::getUserItemListOrderbyRegdt()
{
    CCArray* result = CCArray::create();
    for (int i=list->count()-1; i>=0; i--) {
        result->addObject(list->objectAtIndex(i));
    }
    return result;
}

USERITEM_ADD_RESULT UserItemFactory::addUserItem(ViewItem *item)
{
    USERITEM_CHECK_RESULT checlResult = checkUserItem(item);
    if ( checlResult==USERITEM_CHECK_RESULT_ALREADY ) {
        return ALREADY;
    }
    if ( checlResult==USERITEM_CHECK_RESULT_MAX ) {
        return COUNTMAX;
    }
    CCArray* addList = getUserItemDicList();
    addList->addObject(createDicFromItem(item));
    CCLOG("ItemAdd after size %d", addList->count());
    mainDic->setObject(addList, KEY_NAME_USERITEM_LIST);
    saveDataForFile();
    init();
    return SUCCESS;
}

void UserItemFactory::deleteUserItem(ViewItem *item)
{
    CCArray* ary = getUserItemList();
    int index = -1;
    for (int i=0; i<ary->count(); i++) {
        ViewItem* addedItem = (ViewItem*)ary->objectAtIndex(i);
        if ( !addedItem ) {
            continue;
        }
        if ( strcmp(item->getItemid()->getCString(), addedItem->getItemid()->getCString())==0 ) {
            index = i;
            break;
        }
    }
    if (index!=-1) ary->removeObjectAtIndex(index);
    //保存用の配列作成
    CCArray* saveArray = CCArray::create();
    for (int i=0; i<ary->count(); i++) {
        saveArray->addObject(createDicFromItem((ViewItem*)ary->objectAtIndex(i)));
    }
    mainDic->setObject(saveArray, KEY_NAME_USERITEM_LIST);
    saveDataForFile();
    init();
}

USERITEM_CHECK_RESULT UserItemFactory::checkUserItem(ViewItem *item)
{
    CCArray* ary = getUserItemList();
    
    if ( ary->count()>=MAX_COUNT_USER_ITEM ) {
        return USERITEM_CHECK_RESULT_MAX;
    }
    
    for (int i=0; i<ary->count(); i++) {
        ViewItem* addedItem = (ViewItem*)ary->objectAtIndex(i);
        if ( !addedItem ) {
            continue;
        }
        CCLOG("item is %s", item->getItemid()->getCString());
        CCLOG("addedItem is %s", addedItem->getItemid()->getCString());
        if ( strcmp(item->getItemid()->getCString(), addedItem->getItemid()->getCString())==0 ) {
            return USERITEM_CHECK_RESULT_ALREADY;
        }
    }
    return USERITEM_CHECK_RESULT_OK;
}

CCDictionary* UserItemFactory::createDicFromItem(ViewItem *item)
{
    CCDictionary* dic = CCDictionary::create();
    dic->setObject(item->getItemid(),KEY_NAME_USERITEM_DIC_ITEMID);
    dic->setObject(item->getTitle(),KEY_NAME_USERITEM_DIC_TITLE);
    dic->setObject(item->getAuther(),KEY_NAME_USERITEM_DIC_AUTHER);
    dic->setObject(item->getCreator(),KEY_NAME_USERITEM_DIC_CREATOR);
    dic->setObject(item->getLabel(),KEY_NAME_USERITEM_DIC_LABEL);
    dic->setObject(item->getReleaseDate(),KEY_NAME_USERITEM_DIC_RELEASEDATE);
    dic->setObject(item->getPrice(),KEY_NAME_USERITEM_DIC_PRICE);
    dic->setObject(item->getContents(),KEY_NAME_USERITEM_DIC_CONTENTS);
    dic->setObject(item->getImgLarge(),KEY_NAME_USERITEM_DIC_IMGLARGE);
    dic->setObject(item->getImgSmall(),KEY_NAME_USERITEM_DIC_IMGSMALL);
    CCString* url = item->getUrl();
    url = (url==NULL) ? CCString::create(""): url;
    dic->setObject(url,KEY_NAME_USERITEM_DIC_SHOPURL);
    return dic;
}

ViewItem* UserItemFactory::createItemFromDic(cocos2d::CCDictionary *dic)
{
    ViewItem* item = new ViewItem();
    item->setItemid((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_ITEMID));
    item->setTitle((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_TITLE));
    item->setAuther((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_AUTHER));
    item->setCreator((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_CREATOR));
    item->setLabel((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_LABEL));
    item->setReleaseDate((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_RELEASEDATE));
    item->setPrice((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_PRICE));
    item->setContents((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_CONTENTS));
    item->setImgLarge((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_IMGLARGE));
    item->setImgSmall((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_IMGSMALL));
    item->setImgSmall((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_IMGSMALL));
    item->setUrl((CCString*)dic->objectForKey(KEY_NAME_USERITEM_DIC_SHOPURL));
    return item;
}

void UserItemFactory::saveDataForFile()
{
    CCLog("Save2");
    if (mainDic->writeToFile(UserDataDefault::sharedInstance()->getBookMarkSaveFilePath().c_str()) ) {
        CCLOG("UserItemFactory is Saved %s", UserDataDefault::sharedInstance()->getBookMarkSaveFilePath().c_str());
    }
}

