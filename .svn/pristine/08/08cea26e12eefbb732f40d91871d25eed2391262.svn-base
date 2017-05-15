//
//  SubCharactor.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/04.
//
//

#ifndef __Ranotan2D__SubCharactor__
#define __Ranotan2D__SubCharactor__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace SubCharactorStatus {
    enum Status
    {
        Status_Normal = 0,
        Status_Rolling_After = 1,
        Status_Jump = 2,
        Status_StarLight = 3,
        Status_JumpDance = 4,
        Status_Tutorial_PageMove = 5,
        Status_Tutorial_Bookmart_Add = 6,
        Status_Jump_Check = 7,
    };
}

#define TAG_ANIM_NORMAL (110001)
#define TAG_ANIM_FINGER (110002)

class SubCharactor : public CCSprite
{
public:
    SubCharactor();
    virtual ~SubCharactor();
    virtual bool init(CCPoint positoiin);
    
    static SubCharactor* create(CCPoint positoiin);
    
    void changeAnimation( SubCharactorStatus::Status subCharactorStatus );
    
    void deleteNotification();
    
private:
    CCPoint basePos;
    SubCharactorStatus::Status status;
    //サブキャラアニメーション
    inline void changeAnimationJump() { changeAnimation(SubCharactorStatus::Status_Jump); }
    inline void changeAnimationJumpCheck() { changeAnimation(SubCharactorStatus::Status_Jump_Check); }
    inline void changeAnimationRollingAfter() { changeAnimation(SubCharactorStatus::Status_Rolling_After); }
    inline void changeAnimationNormal() { changeAnimation(SubCharactorStatus::Status_Normal); }
    inline void changeAnimationStarLight() { changeAnimation(SubCharactorStatus::Status_StarLight); }
    inline void changeAnimationJumpDance() { changeAnimation(SubCharactorStatus::Status_JumpDance); }
    inline void changeAnimationTutorialPageMove() { changeAnimation(SubCharactorStatus::Status_Tutorial_PageMove); }
    
    void createAngre();
    void animStarLight1();
};


#endif /* defined(__Ranotan2D__SubCharactor__) */
