//
//  ReflectParticle.h
//  Ranotan2D
//
//  Created by talestaka on 2015/04/16.
//
//

#ifndef __Ranotan2D__ReflectParticle__
#define __Ranotan2D__ReflectParticle__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Reflectparticle : public CCNode
{
public:
    Reflectparticle();
    virtual ~Reflectparticle();
    virtual bool init(CCPoint startPoint);
    
    static Reflectparticle* create(CCPoint startPoint);
    void doMove();
    
    CC_SYNTHESIZE(int, speedX, SpeedX);
    CC_SYNTHESIZE(int, speedY, SpeedY);
    CC_SYNTHESIZE(int,  seekX, SeekX);
    CC_SYNTHESIZE(int,  seekY, SeekY);

    CC_SYNTHESIZE(bool, move, Move);
    
private:
    void update(float delta);

};

#endif /* defined(__Ranotan2D__ReflectParticle__) */
