//
//  AsyncLabelNotice.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/09/01.
//
//

#ifndef __Ranotan2D__AsyncLabelNotice__
#define __Ranotan2D__AsyncLabelNotice__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include "../jsoncpp/json/json.h"

class AsyncLabelNotice : public CCNode
{
public:
    AsyncLabelNotice();
    virtual ~AsyncLabelNotice();
    virtual bool init(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc
                      );
    static AsyncLabelNotice* create(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc);
    
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
        
private:
    
};




#endif /* defined(__Ranotan2D__AsyncLabelNotice__) */
