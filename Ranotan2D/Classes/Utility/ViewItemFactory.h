//
//  ViewItemFactory.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/10.
//
//

#ifndef __Ranotan2D__ViewItemFactory__
#define __Ranotan2D__ViewItemFactory__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "../jsoncpp/json/json.h"

class ViewItem :public CCObject
{
public:
    CC_SYNTHESIZE_RETAIN(CCString*, itemid, Itemid);
    CC_SYNTHESIZE_RETAIN(CCString*, title, Title);
    CC_SYNTHESIZE_RETAIN(CCString*, auther, Auther);
    CC_SYNTHESIZE_RETAIN(CCString*, creator, Creator);
    CC_SYNTHESIZE_RETAIN(CCString*, label, Label);
    CC_SYNTHESIZE_RETAIN(CCString*, releasedate, ReleaseDate);
    CC_SYNTHESIZE_RETAIN(CCString*, price, Price);
    CC_SYNTHESIZE_RETAIN(CCString*, contents, Contents);
    CC_SYNTHESIZE_RETAIN(CCString*, imgLarge, ImgLarge);
    CC_SYNTHESIZE_RETAIN(CCString*, imgSmall, ImgSmall);
    CC_SYNTHESIZE_RETAIN(CCString*, url, Url);
};

class ViewItemFactory : public CCObject
{
public:
    CC_SYNTHESIZE_RETAIN(CCDictionary*, mainDic, MainDic);
    CC_SYNTHESIZE_RETAIN(CCDictionary*, tempMainDic, TempMainDic);

    ViewItemFactory();
    virtual ~ViewItemFactory();    
    virtual bool init();
    CREATE_FUNC(ViewItemFactory);
    
    static ViewItemFactory* sharedInstance();
    
    std::string getItemList();
    std::string getItemListFromTemp();
    CCString* getImageItem(CCString* itemId, CCString* imageType);
    CCArray* getItemListForJson();
    CCArray* getItemListForJsonOrderbyReleaseDate();
    CCArray* getItemListForJsonFromTemp();
    CCArray* createItemList(Json::Value m_root);
    CCArray* createSearchList();
    
    void updateItemList(std::string itemList);
    void updateSearchList(std::string itemList);
    void updateImageItem(CCString* item, CCString* itemId, CCString* imageType);
    
    void saveDataForFile();
    
    void updateCommit();

protected:
    Json::Value m_root;

private:
    
    ViewItem* createViewItem(Json::Value &m_json);
    CCArray* doComparate(CCArray* ary, ViewItem* item);
};

#endif /* defined(__Ranotan2D__ViewItemFactory__) */
