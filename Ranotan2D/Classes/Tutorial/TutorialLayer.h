//
//  TutorialLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/05/30.
//
//

#ifndef __Ranotan2D__TutorialLayer__
#define __Ranotan2D__TutorialLayer__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class TutorialLayer : public CCLayer
{
public:
    TutorialLayer();
    virtual ~TutorialLayer();
    virtual bool init(CCNode* node);
    static TutorialLayer* create(CCNode* node);
    
    void createNode(CCNode* node);
};


#endif /* defined(__Ranotan2D__TutorialLayer__) */
