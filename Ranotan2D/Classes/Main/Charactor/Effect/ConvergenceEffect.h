//
//  ConvergenceEffect.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/05.
//
//

#ifndef __Ranotan2D__ConvergenceEffect__
#define __Ranotan2D__ConvergenceEffect__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ConvergenceEffectLayer : public CCLayer
{
public:
    CC_SYNTHESIZE(CCNode*, node, Node);
    CC_SYNTHESIZE(CCSprite*, sprite, Sprite);
    CC_SYNTHESIZE(CCPoint, centerPos, CentetPos);
    CC_SYNTHESIZE(float, widthR, WidthR);
    CC_SYNTHESIZE(float, lifeTime, LifeTIme);
    
    ConvergenceEffectLayer();
    virtual ~ConvergenceEffectLayer();
    virtual bool init();

    CREATE_FUNC(ConvergenceEffectLayer);
    
    void update(float delta);

    void go();
    void createConvergenceSprite(CCSprite* sprite, float delay);
private:
    void stopSchedule();
};

class ConvergenceEffect : public CCSprite
{
public:
    CC_SYNTHESIZE(CCPoint, centerPos, CentetPos);
    CC_SYNTHESIZE(float, widthR, WidthR);
    ConvergenceEffect();
    virtual ~ConvergenceEffect();
    virtual bool init();
    
    static ConvergenceEffect* create();
    
    void go();

};

#endif /* defined(__Ranotan2D__ConvergenceEffect__) */
