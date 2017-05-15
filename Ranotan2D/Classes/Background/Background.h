//
//  Background.h
//  Ranotan2D
//
//  Created by talestaka on 2015/03/12.
//
//

#ifndef __Ranotan2D__Background__
#define __Ranotan2D__Background__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "AppController.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Background : public cocos2d::CCNode {
public:
    Background();
    virtual ~Background();
    
    virtual bool init();
//    virtual bool init(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc);
    static Background* create();
//    static Background* create(CCObject* target, SEL_CallFunc callfunc, CCObject* failTarget, SEL_CallFunc failCallfunc);

    void onHttpRequestCompleted(CCNode *sender, void *data);
    
    std::string getMessage();
};



#endif /* defined(__Ranotan2D__Background__) */
