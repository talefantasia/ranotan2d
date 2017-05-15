//
//  ImageAsyncSprite.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/14.
//
//

#include "ImageAsyncSprite.h"

ImageAsyncSprite::ImageAsyncSprite()
{
    imageSize = CCSizeZero;
    name="";
}

ImageAsyncSprite::~ImageAsyncSprite()
{
    imageSize = CCSizeZero;
    name="";
}

bool ImageAsyncSprite::init(const char* fileName,const char* url, CCSize size)
{
    //仮の画像を出しておく。
    if ( !CCSprite::initWithSpriteFrameName("img_detail_noimage.png") ) {
        return false;
    }
    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    if ( cache->textureForKey(fileName)==NULL ) {
        CCLog("get Async");
        //Http通信開始
        getHttpAsync(url);
        imageSize = size;
        name = fileName;
    } else {
        CCLog("get chace");
        CCTexture2D* texture =  cache->textureForKey(fileName);
        CCSprite::initWithTexture(texture);
        CCSize contentSize = getContentSize();
        float scaleX = imageSize.width / contentSize.width;
        float scaleY = imageSize.height / contentSize.height;
        setScaleX(scaleX);
        setScaleY(scaleY);
    }
    
    return true;
}

ImageAsyncSprite* ImageAsyncSprite::create(const char* fileName,const char *url, CCSize size)
{
    ImageAsyncSprite* pRet = new ImageAsyncSprite();
    if (pRet && pRet->init(fileName, url, size)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void ImageAsyncSprite::getHttpAsync(const char *url)
{
    CCLog(url);
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(url);
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(ImageAsyncSprite::onHttpRequestCompleted)); // コールバック
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void ImageAsyncSprite::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse *) data;
    if (!response->isSucceed()) return;
    
    std::vector<char> *buffer = response->getResponseData();
        
    // 2. CCImageのinitWithImageDataで画像フォーマットを解析
    CCImage *image = new CCImage();
    image->initWithImageData(&(buffer->front()), buffer->size());
    
    // 3. CCTexture2DからCCSpriteへ
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithImage(image);
    
    CCTextureCache::sharedTextureCache()->addUIImage(image, name);
//    setContentSize(texture->getContentSize());
//    setTexture(texture);
    CCSprite::initWithTexture(texture);
    CCSize contentSize = getContentSize();
    float scaleX = imageSize.width / contentSize.width;
    float scaleY = imageSize.height / contentSize.height;
    setScaleX(scaleX);
    setScaleY(scaleY);

    
    
}