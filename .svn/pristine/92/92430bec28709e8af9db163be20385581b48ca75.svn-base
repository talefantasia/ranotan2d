//
//  TextureFactory.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/22.
//
//

#include "TextureFactory.h"
#include "ViewItemFactory.h"
#include "UserDataDefault.h"

TextureFactory* textureFactory = NULL;

TextureFactory::TextureFactory()
{
    index = 0;
    startIndex = 0;
    endIndex = 0;
    currentIndex = 0;
    textures = NULL;
    texturesTemp = NULL;
    itemList = NULL;
}

TextureFactory::~TextureFactory()
{
    index = 0;
    startIndex = 0;
    endIndex = 0;
    currentIndex = 0;
    textures = NULL;
    texturesTemp = NULL;
    itemList = NULL;
}

bool TextureFactory::init()
{
    resetTextures();
    return true;
}

TextureFactory* TextureFactory::shardInstance()
{
    if (!textureFactory) {
        textureFactory = TextureFactory::create();
        CC_SAFE_RETAIN(textureFactory);
    }
    return textureFactory;
}

void TextureFactory::initTextures(CCArray* list, int idx)
{
    itemList=list;
    index = idx;

    //開始Indexの設定
    startIndex = index-SIZE_LOWER_RANGE;
    if (startIndex<0) startIndex=0;
    //終了Indexの設定
    endIndex = index + SIZE_UPPER_RANGE;
    if ( endIndex>=itemList->count()) endIndex = itemList->count()-1;
    currentIndex = startIndex;
    //テクスチャのリセット
    resetTextures();
    //新規に作り始める
    createTextures();
}

void TextureFactory::createTextures()
{
    //最後のIndexを超えたら戻る。
    if ( endIndex<currentIndex ) {
        return;
    }
    
    ViewItem* item = (ViewItem*)itemList->objectAtIndex(currentIndex);
    
    std::string file = UserDataDefault::sharedInstance()->getNowTargetDirPath()+ "L_" +item->getItemid()->getCString();
    
    CCLOG("Async Image Load Start %s", file.c_str());
    
    CCTextureCache* cache = CCTextureCache::sharedTextureCache();
    cache->addImageAsync(file.c_str(), this, callfuncO_selector(TextureFactory::loadedImage));
}

void TextureFactory::loadedImage(cocos2d::CCObject *object)
{
    CCTexture2D* texture = (CCTexture2D*)object;
    CCLOG("Async Image Load End");
    textures->addObject(texture);
    currentIndex++;
    createTextures();
}

void TextureFactory::resetTextures()
{
    textures = CCArray::createWithCapacity(SIZE_TEXTURES);
}