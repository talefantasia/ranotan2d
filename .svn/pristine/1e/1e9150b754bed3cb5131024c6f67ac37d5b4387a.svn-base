//
//  StarFlyEffect.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/12.
//
//

#ifndef __Ranotan2D__StarFlyEffect__
#define __Ranotan2D__StarFlyEffect__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class StarFlyEffect : public CCSprite
{
public:
    StarFlyEffect();
    virtual ~StarFlyEffect();
    virtual bool init();
    
    static StarFlyEffect* create();
    
    void initActionRundom(int seed);
    void initAction(int deg, float speed, float width, float scale);

    
private:
    void removeMySelf();
};

#endif /* defined(__Ranotan2D__StarFlyEffect__) */
