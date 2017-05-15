//
//  AsyncSearchItem.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/02/07.
//
//

#ifndef __Ranotan2D__AsyncSearchItem__
#define __Ranotan2D__AsyncSearchItem__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "../jsoncpp/json/json.h"

class AsyncSearchItem : public CCNode
{
public:
    AsyncSearchItem();
    virtual ~AsyncSearchItem();
    virtual bool init(CCObject* target, SEL_CallFunc callfunc, CCDictionary* params);
    static AsyncSearchItem* create(CCObject* target, SEL_CallFunc callfunc, CCDictionary* params);
    
    CC_SYNTHESIZE(CCArray*, itemList, ItemList);
    
    void doImageSync();
    void process1();
    void process1Complete(CCNode *sender, void *data);
    void process2();
    void process2Complete(CCNode *sender, void *data);

    
    int index;
    CCObject* m_pTarget;
    SEL_CallFunc m_pCallfunc;
    CCDictionary* m_pParams;

private:
    void doSync();
    void onHttpRequestCompleted(CCNode *sender, void *data);
    void doBeforeCloseSyncProcess();
    void doCloseSyncProcess();
    void doBeforeCancelSyncProcess();
    void doCancelSyncProcess();

    std::string UrlEncode( const std::string & str );

};

#endif /* defined(__Ranotan2D__AsyncSearchItem__) */
