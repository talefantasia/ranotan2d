//
//  ResolutionUtil.h
//  zuma
//
//  Created by KASAI Masahide on 2013/04/05.
//
//

#ifndef __zuma__ResolutionUtil__
#define __zuma__ResolutionUtil__

#include <cocos2d.h>

USING_NS_CC;

#define MAX_WIDTH (float) 960
#define MAX_HEIGHT (float) 640
#define MIN_WIDTH (float) 480
#define MIN_HEIGHT (float) 320

class ResolutionUtil : public CCObject
{
public:
    static inline float getScaleRate()
    {
        //return CCDirector::sharedDirector()->getWinSize().width / MAX_WIDTH;
        return 1.0f;
    }
    static inline CCSize getSize(float width,float height)
    {
		return CCSize(width * getScaleRate(),height * getScaleRate());
    }
    
    static inline CCPoint getPosition(CCPoint point)
    {
        float x = point.x * getScaleRate() + getOffsetX();
        float y = point.y * getScaleRate() + getOffsetY();
        return ccp(x,y);
    }
    
    static inline CCPoint reverseToCocosPosition(CCPoint point)
    {
        float x = point.x;
        float y = (MAX_HEIGHT - point.y);
        return ccp(x + getOffsetX(),y + getOffsetY());
    }
    static inline float getSpeed(float speed)
    {
        return speed * getScaleRate();
    }
    
    static inline float getOffsetY() {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        return ( winSize.height - (MAX_HEIGHT * getScaleRate()) ) / 2.0f;
    }
	
    static inline float getOffsetX() {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        float scaleRate = CCDirector::sharedDirector()->getWinSize().height / MAX_HEIGHT;
        return ( winSize.width - (MAX_WIDTH * scaleRate) ) / 2.0f;
    }
    
};

#endif /* defined(__zuma__ResolutionUtil__) */
