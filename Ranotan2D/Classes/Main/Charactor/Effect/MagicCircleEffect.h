//
//  MagicCircleEffect.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/20.
//
//

#ifndef __Ranotan2D__MagicCircleEffect__
#define __Ranotan2D__MagicCircleEffect__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace MagicCircleEffectStatus {
    enum ColorType
    {
        ColorType_GREEN   = 1,
        ColorType_BLUE    = 2,
        ColorType_PURPLE  = 3,
        ColorType_NONE  = 4,
        ColorType_NONE2  = 5,
    };
    
    enum AnimationType
    {
        AnimationType_Normal  = 1,
        AnimationType_TYPE1  = 2,
    };

    enum StarMoveType
    {
        StarMoveType_IN  = 1,
        StarMoveType_OUT  = 2,
    };

};

class MagicCircleEffect : public CCNode
{
public:
    
    MagicCircleEffect();
    virtual ~MagicCircleEffect();
    
    virtual bool init(MagicCircleEffectStatus::ColorType color, MagicCircleEffectStatus::AnimationType animation, MagicCircleEffectStatus::StarMoveType starMove);
    static MagicCircleEffect* create(MagicCircleEffectStatus::ColorType color, MagicCircleEffectStatus::AnimationType animation, MagicCircleEffectStatus::StarMoveType starMove);
    
    
    CC_SYNTHESIZE(MagicCircleEffectStatus::ColorType, color, Color);
    CC_SYNTHESIZE(MagicCircleEffectStatus::AnimationType, animation, Animation);
    CC_SYNTHESIZE(MagicCircleEffectStatus::StarMoveType, starMove, StarMove);
    
private:
    void initCircleEffect();
    void initStarEffect();
    void createStar(const char* filename, const char* actionName, CCPoint movePos, float delay);
    void createStar2(const char* filename, const char* actionName, CCPoint movePos, float delay);
    void createStar3(const char* filename, const char* actionName, CCPoint movePos, float delay);
    
};

#endif /* defined(__Ranotan2D__MagicCircleEffect__) */
