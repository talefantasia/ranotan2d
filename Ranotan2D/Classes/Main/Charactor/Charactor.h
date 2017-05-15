//
//  Charactor.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/05.
//
//

#ifndef __Ranotan2D__Charactor__
#define __Ranotan2D__Charactor__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define TAG_CHARACTOR_EUBEFFECT (80001)
#define TAG_CHARACTOR_STARPOS   (80002)

namespace CharactorStatus {
    enum Status
    {
        Status_Normal   = 1,
    };
    
    enum AnimationStatus
    {
        AnimationStatus_Fly         = 1,
        AnimationStatus_Rotate_right= 2,
        AnimationStatus_Rotate_left = 3,
        AnimationStatus_Rotate = 4,
        AnimationStatus_Rolling = 5,
        AnimationStatus_StarLight = 6,
        AnimationStatus_JumpDance = 7,
        AnimationStatus_Tap = 8,
        AnimationStatus_ChangeDetail = 9,
        AnimationStatus_Paropunte = 10,
    };
    
    enum EffectStatus
    {
        None            = 0,
        Circle_Green    = 1,
        Circle_Blue     = 2,
        Circle_Purple   = 3,
    };
    
    enum SubEffect
    {
        SubEffect_None = 0,
        SubEffect_Search = 1,
        SubEffect_Search_Image = 2,
        SubEffect_Search_complete = 3,
        SubEffect_Search_Cancel = 4,
    };
}

class Charactor : public CCSprite
{
public:
    Charactor();
    virtual ~Charactor();
    virtual bool init(const char* filename);
    
    static Charactor* create(const char* filename);
    void setStatus(CharactorStatus::Status status);
    void addSubAnimation(CharactorStatus::AnimationStatus animationStatus);
    void addSubEffect(CharactorStatus::SubEffect subEffect);
    
    void getSubCharactorWorldPosition();
    
private:
    CharactorStatus::Status status;
    CharactorStatus::AnimationStatus animation;
    void changeAnimation();
    void resetAnimationStatus();
    inline void addSubEffectSearch() { addSubEffect(CharactorStatus::SubEffect_Search); };
    inline void addSubEffectSearchImage() { addSubEffect(CharactorStatus::SubEffect_Search_Image); };
    inline void addSubEffectSearchComplete() { addSubEffect(CharactorStatus::SubEffect_Search_complete); };
    inline void addSubEffectSearchCancel() { addSubEffect(CharactorStatus::SubEffect_Search_Cancel); };
    inline void addSubEffectNone() { addSubEffect(CharactorStatus::SubEffect_None); };
    
    //キャラクターアニメーション関連
    void animStarLight1();
    void animStarLight2();
    void animStarLight3();
    void animChangeDetail();
    void animParopunte1();
    void animParopunte2();
};

#endif /* defined(__Ranotan2D__Charactor__) */
