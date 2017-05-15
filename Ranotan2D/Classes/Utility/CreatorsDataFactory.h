//
//  CreatorFactory.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/06/21.
//
//

#ifndef __Ranotan2D__CreatorsDataFactory__
#define __Ranotan2D__CreatorsDataFactory__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum CREATOR_TYPE
{
    CREATOR_TYPE_AUTHOR = 0,
    CREATOR_TYPE_CREATOR = 1,
};

class CreatorsDataFactory : public CCObject
{
public:
    
    CC_SYNTHESIZE_RETAIN(CCArray*, plans, Plans);
    CC_SYNTHESIZE_RETAIN(CCArray*, bookmarks, Bookmarks);
    CC_SYNTHESIZE_RETAIN(CCDictionary*, authors, Authors);
    CC_SYNTHESIZE_RETAIN(CCDictionary*, creators, Creators);

    CC_SYNTHESIZE(int, listIndex, ListIndex);
    
    CreatorsDataFactory();
    virtual ~CreatorsDataFactory();
    virtual bool init();
    CREATE_FUNC(CreatorsDataFactory);
    static CreatorsDataFactory* sharedInstance();
    
    //リストの取得
    inline CCDictionary* getAuthorList() { return getCreatorsList(CREATOR_TYPE_AUTHOR); }
    inline CCDictionary* getCreatorList(){ return getCreatorsList(CREATOR_TYPE_CREATOR); }
    
    CCArray* getCheckList();
    
private:
    CCDictionary* getCreatorsList(CREATOR_TYPE type);

};

#endif /* defined(__Ranotan2D__CreatorsDataFactory__) */
