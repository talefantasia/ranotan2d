//
//  AsyncImageItem.cpp
//  Ranotan2D
//
//  Created by 高橋 弘幸 on 2013/12/17.
//
//

#include "AsyncImageItem.h"
#include "ViewItemFactory.h"
#include "ViewItemAsyncProcesser.h"
#include "UserDataDefault.h"

//const std::string SavePath = CCFileUtils::sharedFileUtils()->getWritablePath();
const std::string dir1 = "dic1/";
const std::string dir2 = "dic2/";

AsyncImageItem::AsyncImageItem()
{
    m_pTarget=NULL;
    m_pCallfunc=NULL;
    index = 0;
    itemList = NULL;
}

AsyncImageItem::~AsyncImageItem()
{
    m_pTarget=NULL;
    m_pCallfunc=NULL;
    index = 0;
    itemList = NULL;
}


bool AsyncImageItem::init()
{
    itemList=ViewItemFactory::sharedInstance()->getItemListForJsonFromTemp();
    if (itemList!=NULL) itemList->retain();
    doSync();
    return true;
}

bool AsyncImageItem::init(CCObject* target, SEL_CallFunc callfunc)
{
    itemList=ViewItemFactory::sharedInstance()->getItemListForJsonFromTemp();
    if (itemList!=NULL) itemList->retain();
    m_pTarget=target;
    m_pCallfunc=callfunc;
    sendMessage("Image Data Sync Start! ( Process 3/4 )", false);
    doSync();
    return true;
}


AsyncImageItem* AsyncImageItem::create()
{
    AsyncImageItem* pRet = new AsyncImageItem();
    if (pRet && pRet->init()) {
        pRet->retain();
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }

}

AsyncImageItem* AsyncImageItem::create(CCObject* target, SEL_CallFunc callfunc)
{
    AsyncImageItem* pRet = new AsyncImageItem();
    if (pRet && pRet->init(target, callfunc)) {
        pRet->retain();
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
    
}

void AsyncImageItem::doSync()
{
 
    if ( itemList==NULL ) {
        return;
    }
    if ( itemList->count()==0 ) {
        return;
    }
    if ( index>=itemList->count() ) {
        doDelegate();
        return;
    }
    
    
    process1();

}

void AsyncImageItem::process1()
{
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    CCString* str = CCString::createWithFormat("%d / %d complete", index, itemList->count());
    sendMessage(str->getCString(), true);
    
    if (!item->getImgLarge()) {
        CCLog("index %d is blank", index);
        index++;
        doSync();
        return;
    }
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(item->getImgLarge()->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(AsyncImageItem::process1Complete)); // コールバック
    CCHttpClient::getInstance()->setTimeoutForRead(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
    CCLog("request Large Image %s", item->getImgLarge()->getCString());
}

void AsyncImageItem::process1Complete(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse *) data;
    if (!response->isSucceed()) {
        index++;
        doSync();
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    
    if ( buffer==NULL || buffer->size()==0 ) {
        index++;
        doSync();
        return;
    }
    
    std::string imgBuffer(buffer->begin(), buffer->end());
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    
    CCImage *image = new CCImage();
    image->initWithImageData(&(buffer->front()), buffer->size());
    
    image->saveToFile((UserDataDefault::sharedInstance()->getNowTargetDirPath() + "L_" + item->getItemid()->getCString()).c_str());
    
    process2();
}

void AsyncImageItem::process2()
{
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    CCString* str = CCString::createWithFormat("%d / %d complete", index, itemList->count());
    sendMessage(str->getCString(), true);
    
    if (!item->getImgLarge()) {
        CCLog("index %d is blank", index);
        index++;
        doSync();
        return;
    }
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(item->getImgSmall()->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(AsyncImageItem::process2Complete)); // コールバック
    CCHttpClient::getInstance()->setTimeoutForRead(10);
    CCHttpClient::getInstance()->send(request);
    request->release();
    CCLog("request Small Image %s", item->getImgSmall()->getCString());
}

void AsyncImageItem::process2Complete(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse *) data;
    if (!response->isSucceed()) {
        index++;
        doSync();
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    
    if ( buffer==NULL || buffer->size()==0 ) {
        index++;
        doSync();
        return;
    }
    
    std::string imgBuffer(buffer->begin(), buffer->end());
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(index);
    
    CCImage *image = new CCImage();
    image->initWithImageData(&(buffer->front()), buffer->size());
    
    image->saveToFile((UserDataDefault::sharedInstance()->getNowTargetDirPath() + "S_" + item->getItemid()->getCString()).c_str());
    index++;
    doSync();
}

void AsyncImageItem::sendMessage(const char *message, bool isCurrent)
{
    ViewItemAsyncProcesser* processor = dynamic_cast<ViewItemAsyncProcesser*>(m_pTarget);
    if ( processor==NULL ) {
        return;
    }
    processor->sendMessage(message, isCurrent);
}

void AsyncImageItem::doDelegate()
{
    sendMessage("Image Data Sync End! ( Process 3/4 )", false);
    //CallBack
    if ( m_pTarget && m_pCallfunc ) {
        (m_pTarget->*m_pCallfunc)();
    }
}
