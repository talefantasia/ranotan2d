//
//  AsyncItemList.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/17.
//
//

#ifndef __Ranotan2D__AsyncItemList__
#define __Ranotan2D__AsyncItemList__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "../jsoncpp/json/json.h"

class AsyncItemList : public CCNode
{
public:
    AsyncItemList();
    virtual ~AsyncItemList();
    virtual bool init();
    virtual bool init(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc
                      );
    static AsyncItemList* create();
    static AsyncItemList* create(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc);
    
    void doAsync();
    void onHttpRequestCompleted(CCNode *sender, void *data);

    CCObject* m_pTarget;
    SEL_CallFunc m_pCallfunc;
    CCObject* m_pFailTarget;
    SEL_CallFunc m_pFailCallfunc;
    
    unsigned int index;
    
protected:
    Json::Value m_root;
    
//    void doBeforeCloseSyncProcess();
//    void doCloseSyncProcess();
    
    void sendMessage(const char* message, bool isCurrent);

    
private:
    
};

#endif /* defined(__Ranotan2D__AsyncItemList__) */
