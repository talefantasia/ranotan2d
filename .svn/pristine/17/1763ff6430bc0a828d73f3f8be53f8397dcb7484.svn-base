//
//  Background.cpp
//  Ranotan2D
//
//  Created by talestaka on 2015/03/12.
//
//

#include "Background.h"

#define SERVER_URL_ITEMLIST "http://talefantasia.appspot.com/search/plan2?version=2"

Background::Background()
{
    CCLOG("Bg Fetch Constracturd");
}

Background::~Background()
{
    CCLOG("Bg Fetch Destracturd");
}


bool Background::init()
{    
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(SERVER_URL_ITEMLIST);
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(Background::onHttpRequestCompleted)); // コールバック
    CCHttpClient::getInstance()->setTimeoutForRead(30);
    CCHttpClient::getInstance()->send(request);
    request->release();

    
    return true;
}

Background* Background::create()
{
    Background* background = new Background();
    background->init();
    return background;
    
}

std::string Background::getMessage()
{
    CCLOG("Bg Fetch getMessage");
    std::string str("message");
    return str;
}

void Background::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCLOG("Callback RequestComplete");
    this->release();
}