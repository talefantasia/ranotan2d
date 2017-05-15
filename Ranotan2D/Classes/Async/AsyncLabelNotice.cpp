//
//  AsyncLabelNotice.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/09/01.
//
//

#include "AsyncLabelNotice.h"

#define SERVER_URL_LABEL_NOTICE "http://talefantasia.appspot.com/notice/labelnotice?version=1&type=ios"

AsyncLabelNotice::AsyncLabelNotice()
{
}

AsyncLabelNotice::~AsyncLabelNotice()
{
}

bool AsyncLabelNotice::init(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc)
{
    m_pTarget = target;
    m_pCallfunc = callfunc;
    m_pFailTarget = failTarget;
    m_pFailCallfunc=failCallfunc;
    doAsync();
    return true;
}

AsyncLabelNotice* AsyncLabelNotice::create(cocos2d::CCObject *target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc)
{
    AsyncLabelNotice* pRet = new AsyncLabelNotice();
    if (pRet && pRet->init(target, callfunc, failTarget, failCallfunc)) {
        pRet->retain();
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void AsyncLabelNotice::doAsync()
{
    CCLog("Start AsynItemList");
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(SERVER_URL_LABEL_NOTICE);
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(AsyncLabelNotice::onHttpRequestCompleted)); // コールバック
    CCLog("%d",CCHttpClient::getInstance()->getTimeoutForConnect());
    CCLog("%d",CCHttpClient::getInstance()->getTimeoutForRead());
    CCHttpClient::getInstance()->setTimeoutForRead(60);
    CCHttpClient::getInstance()->send(request);
    request->release();
    
}

void AsyncLabelNotice::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse *) data;
    if (!response->isSucceed()) {
//        DialogLayer* layer = DialogLayer::create(DialogLayer::DialogType_FailItemSync, m_pFailTarget, m_pFailCallfunc, NULL);
//        CCDirector::sharedDirector()->getRunningScene()->addChild(layer);
        return;
    }
    
    std::vector<char> *buffer = response->getResponseData();
    
    std::string config(buffer->begin(), buffer->end() );
//    ViewItemFactory::sharedInstance()->updateItemList(config);
    
    //CallBack
    
    CCLog(config.c_str());
    
    if ( m_pTarget && m_pCallfunc ) {
        (m_pTarget->*m_pCallfunc)();
    }
}
