//
//  CharactorLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/05.
//
//

#ifndef __Ranotan2D__CharactorLayer__
#define __Ranotan2D__CharactorLayer__

#import <cocos2d.h>
#import "cocos-ext.h"
#import "Book.h"
#import "MagicCircleEffect.h"
#import "Charactor.h"


USING_NS_CC;
USING_NS_CC_EXT;

#define TAG_CHARACTOR_NODE  (80000)
#define TAG_CHARACTOR       (80001)
#define TAG_SUB_CHARACTOR   (80003)
#define TAG_BOOKS           (80002)

#define ZORDET_CHARACTRO    (3)
#define ZORDER_BOOK         (4)
#define ZORDER_CONVERGENCE  (5)
#define ZORDER_STARFLAY     (2)

#define IMG_FORMAT "Charactor_h_350_shadow.png"


class CharactorLayer :  public CCLayer
{
public:
    
    CCPoint CharactorBasePos;
    CC_SYNTHESIZE(CCNode*, charaNode, CharaNode);
    
    CharactorLayer();
    virtual ~CharactorLayer();
    virtual bool init();
    
    CREATE_FUNC(CharactorLayer);
        
    void createMagicCircleEffect(MagicCircleEffectStatus::ColorType color, MagicCircleEffectStatus::StarMoveType starMove);
    void moveLayer(CCPoint point, float duration);
    void resetPosition();
    void addSubAnimation(CharactorStatus::AnimationStatus animation);
    void addSubEffect(CharactorStatus::SubEffect subEffect);
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    bool isSmall;
    bool isMove;
    
private:
    
    CharactorStatus::AnimationStatus animationStatus;
    float totalDelta;
    float tapDelta;
    float tapCnt;
    
    void createStar();
    void initCharactor();
    void initSubCharactor();
    void initBook();
    Book* createBook(const char* fileName,  float speed, int deg, int zOrder, float widthX, float widthY, float basePosX, float basePosY);
    
    void initStarFlayEffect();
    
    void resetAnimationStatus();
    
    void editMagicalCircle();
    void createParticleEffect(float duration);
    
    void update(float delta);
    void doRandamAnimation();
    void resetTatalDelta();
    
    void startStarLightBreaker();
    void initSLB();
    bool isSLB;
    int totalSLBSize;
    float deltaSLB;
    void updateSLB();
    CC_SYNTHESIZE_RETAIN(CCArray*, stars, Starts);
    
};

#endif /* defined(__Ranotan2D__CharactorLayer__) */
