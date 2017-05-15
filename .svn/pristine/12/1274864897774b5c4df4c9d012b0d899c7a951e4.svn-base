//
//  ViewItemAsyncProcesser.cpp
//  Ranotan2D
//
//  Created by 高橋 弘幸 on 2013/12/18.
//
//

#include "ViewItemAsyncProcesser.h"
#include "ViewItemFactory.h"
#include "ItemViewScene.h"
#include "UserItemFactory.h"
#include "UserDataDefault.h"
#include "CreatorsDataFactory.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

ViewItemAsyncProcesser::ViewItemAsyncProcesser()
{
    m_pTarget=NULL;
    m_pCallfunc=NULL;
    asyncItemList=NULL;
    asyncImageItem=NULL;
}

ViewItemAsyncProcesser::~ViewItemAsyncProcesser()
{
    m_pTarget=NULL;
    m_pCallfunc=NULL;
    CC_SAFE_RELEASE_NULL(asyncItemList);
    CC_SAFE_RELEASE_NULL(asyncImageItem);
}

bool ViewItemAsyncProcesser::init(CCObject* target, SEL_CallFunc callfunc)
{
    m_pTarget=target;
    m_pCallfunc=callfunc;
    //ItemList取得の準備
    sendMessage("Connect Server. ( Process 1/5 )", false);
    UserDataDefault::sharedInstance()->modifyUpdateResult(false);
    setAsyncItemList(AsyncItemList::create(this, callfunc_selector(ViewItemAsyncProcesser::callcackAsyncProcessItemList),m_pTarget, m_pCallfunc));
    return true;
}

ViewItemAsyncProcesser* ViewItemAsyncProcesser::create(CCObject* target, SEL_CallFunc callfunc)
{
    ViewItemAsyncProcesser* pRet = new ViewItemAsyncProcesser();
    if (pRet && pRet->init(target, callfunc)) {
        pRet->retain();
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

void ViewItemAsyncProcesser::sendMessage(const char *message, bool isCurrent)
{
    ServerSyncLayer* layer = dynamic_cast<ServerSyncLayer*>(getParent());
    if ( layer==NULL ) {
        return;
    }
    if ( isCurrent ) {
        layer->addMessageCommon(CCString::create(message), true);
    } else {
        layer->addProcessMessage(CCString::create(message));
    }
}

void ViewItemAsyncProcesser::callcackAsyncProcessItemList()
{
    setAsyncImageItem(AsyncImageItem::create(this, callfunc_selector(ViewItemAsyncProcesser::callbackAsyncProcessImageItem)));
}

void ViewItemAsyncProcesser::callbackAsyncProcessImageItem()
{
    sendMessage("Data Migration Start! ( Process 4/5 )", false);
    ViewItemFactory::sharedInstance()->updateCommit();
    CreatorsDataFactory::sharedInstance()->init();
    //TODO unusing ImageFile Delete
    sendMessage("Data Migration End! ( Process 4/5 )", false);
    
    sendMessage("Image File Cleanup! this is Hevey Load! ( Process 5/5 )", false);
    doImageCleanup();
    //データUPDATe情報の更新
    UserDataDefault::sharedInstance()->modifyUpdateDate();
    UserDataDefault::sharedInstance()->modifyUpdateResult(true);
    if ( m_pTarget && m_pCallfunc ) {
        (m_pTarget->*m_pCallfunc)();
    }
    removeFromParentAndCleanup(true);
}

void ViewItemAsyncProcesser::doImageCleanup()
{
    std::string dir = CCFileUtils::sharedFileUtils()->getWritablePath() + "dir0/";
    
    DIR* dp=opendir(dir.c_str());
    
    int fileNum = 0;
    
    CCDictionary* imageDic= createImageDic();
    
    if (dp!=NULL)
    {
        struct dirent* dent;
        do{
            dent = readdir(dp);
            if (dent!=NULL) {
                fileNum++;
                CCString* type = dynamic_cast<CCString*>(imageDic->objectForKey(dent->d_name));
//                CCObject deleteFlg = doImageCleanupCommon(dent->d_name);
                if ( type==NULL ) {
                    std::string filepath = CCFileUtils::sharedFileUtils()->getWritablePath() + "dir0/" + dent->d_name;
                    int result = remove(filepath.c_str());
                    CCLOG("%s is delete %s", dent->d_name, (result==0)?"SUCCESS":"FAIL");
                }
            }
        }while(dent!=NULL);
        closedir(dp);
    }
   
}

CCDictionary* ViewItemAsyncProcesser::createImageDic()
{
    CCDictionary* dic = CCDictionary::create();
    
    CCArray* list = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    
    dic->setObject(CCString::create("dic"), std::string("."));
    dic->setObject(CCString::create("dic"), std::string(".."));
    
    for (int i=0; i<list->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(list->objectAtIndex(i));
        if ( item==NULL ) {
            continue;
        }
        std::string smallName = "S_" + item->getItemid()->m_sString;
        std::string largeName = "L_" + item->getItemid()->m_sString;
        dic->setObject(CCString::create("plan"), smallName);
        dic->setObject(CCString::create("plan"), largeName);
    }
    
    list = UserItemFactory::sharedInstance()->getUserItemList();
    for (int i=0; i<list->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(list->objectAtIndex(i));
        if ( item==NULL ) {
            continue;
        }
        std::string smallName = "S_" + item->getItemid()->m_sString;
        std::string largeName = "L_" + item->getItemid()->m_sString;
        dic->setObject(CCString::create("bookmark"), smallName);
        dic->setObject(CCString::create("bookmark"), largeName);
    }
    
    return dic;
    
}

bool ViewItemAsyncProcesser::doImageCleanupCommon(std::string filename)
{
    CCArray* list = ViewItemFactory::sharedInstance()->getItemListForJsonOrderbyReleaseDate();
    if ( std::string(".").compare(filename)==0 ) {
        return true;
    }
    
    if ( std::string("..").compare(filename)==0 ) {
        return true;
    }

    for (int i=0; i<list->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(list->objectAtIndex(i));
        if ( item==NULL ) {
            continue;
        }
        std::string smallName = "S_" + item->getItemid()->m_sString;
        std::string largeName = "L_" + item->getItemid()->m_sString;
        if ( filename.compare(smallName)==0 ) {
            return true;
        }
        if ( filename.compare(largeName)==0 ) {
            return true;
        }
    }
    
    list = UserItemFactory::sharedInstance()->getUserItemList();
    for (int i=0; i<list->count(); i++) {
        ViewItem* item = dynamic_cast<ViewItem*>(list->objectAtIndex(i));
        if ( item==NULL ) {
            continue;
        }
        std::string smallName = "S_" + item->getItemid()->m_sString;
        std::string largeName = "L_" + item->getItemid()->m_sString;
        if ( filename.compare(smallName)==0 ) {
            return true;
        }
        if ( filename.compare(largeName)==0 ) {
            return true;
        }
    }
    
    return false;
}
