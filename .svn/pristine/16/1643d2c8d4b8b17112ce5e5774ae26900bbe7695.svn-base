//
//  ViewItemAsyncProcesser.h
//  Ranotan2D
//
//  Created by 高橋 弘幸 on 2013/12/18.
//
//

#ifndef __Ranotan2D__ViewItemAsyncProcesser__
#define __Ranotan2D__ViewItemAsyncProcesser__

#import <cocos2d.h>
#import "cocos-ext.h"

#import "AsyncItemList.h"
#import "AsyncImageItem.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ViewItemAsyncProcesser :public CCNode
{
public:
    ViewItemAsyncProcesser();
    virtual ~ViewItemAsyncProcesser();
    virtual bool init(CCObject* target, SEL_CallFunc callfunc);
    
    static ViewItemAsyncProcesser* create(CCObject* target, SEL_CallFunc callfunc);
    
    void callcackAsyncProcessItemList();
    void callbackAsyncProcessImageItem();
    
    void doImageCleanup();
    bool doImageCleanupCommon(std::string filename);
    CCDictionary* createImageDic();
    
    void callbackAsyncFail();
    
    void sendMessage(const char* message, bool isCurrent);
    
    CCObject* m_pTarget;
    SEL_CallFunc m_pCallfunc;
    
    CC_SYNTHESIZE_RETAIN(AsyncItemList*, asyncItemList, AsyncItemList);
    CC_SYNTHESIZE_RETAIN(AsyncImageItem*, asyncImageItem, AsyncImageItem);
    
};

#endif /* defined(__Ranotan2D__ViewItemAsyncProcesser__) */
