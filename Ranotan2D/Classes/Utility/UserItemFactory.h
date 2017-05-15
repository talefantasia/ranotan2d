//
//  UserItemFactory.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/01/13.
//
//

#ifndef __Ranotan2D__UserItemFactory__
#define __Ranotan2D__UserItemFactory__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "ViewItemFactory.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum USERITEM_ADD_RESULT
{
    SUCCESS = 0,
    ALREADY = 1,
    COUNTMAX = 3,
    FAIL    =2,
};

enum USERITEM_CHECK_RESULT
{
    USERITEM_CHECK_RESULT_OK = 0,
    USERITEM_CHECK_RESULT_ALREADY = 1,
    USERITEM_CHECK_RESULT_MAX = 2,
    
};

class UserItemFactory : public CCObject
{
public:
    CC_SYNTHESIZE_RETAIN(CCDictionary*, mainDic, MainDic);
    CC_SYNTHESIZE_RETAIN(CCArray*, list, List);

    UserItemFactory();
    virtual ~UserItemFactory();
    virtual bool init();
    CREATE_FUNC(UserItemFactory);
    static UserItemFactory* sharedInstance();
    
    USERITEM_ADD_RESULT addUserItem(ViewItem* item);
    void deleteUserItem(ViewItem* item);
    CCArray* createUserItemList();
    CCArray* getUserItemList();
    CCArray* getUserItemListOrderbyRegdt();
    CCArray* getUserItemDicList();
    USERITEM_CHECK_RESULT checkUserItem(ViewItem* item);
    
private:
    CCDictionary* createDicFromItem(ViewItem* item);
    ViewItem* createItemFromDic(CCDictionary* dic);
    void saveDataForFile();
};

#endif /* defined(__Ranotan2D__UserItemFactory__) */
