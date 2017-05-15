//
//  AsyncImageItem.h
//  Ranotan2D
//
//  Created by 高橋 弘幸 on 2013/12/17.
//
//

#ifndef __Ranotan2D__AsyncImageItem__
#define __Ranotan2D__AsyncImageItem__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class AsyncImageItem : public CCObject
{
public:
    AsyncImageItem();
    virtual ~AsyncImageItem();
    virtual bool init();
    virtual bool init(CCObject* target, SEL_CallFunc callfunc);
    
    static AsyncImageItem* create();
    static AsyncImageItem* create(CCObject* target, SEL_CallFunc callfunc);
    
    void doSync();
    void process1();
    void process1Complete(CCNode *sender, void *data);
    void process2();
    void process2Complete(CCNode *sender, void *data);
    
    void doImgLeargeHttpRequest();
    void onImgLeargeHttpRequestCompleted(CCNode *sender, void *data);
    void doDelegate();
    void sendMessage(const char *message, bool isCurrent);

    
    CCArray* itemList;
    CCObject* m_pTarget;
    SEL_CallFunc m_pCallfunc;
    unsigned int index;

};

#endif /* defined(__Ranotan2D__AsyncImageItem__) */
