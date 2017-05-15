//
//  SerifLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/15.
//
//

#ifndef __Ranotan2D__SerifLayer__
#define __Ranotan2D__SerifLayer__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SerifLayer: public CCLayer
{
public:
    SerifLayer();
    virtual ~SerifLayer();
    virtual bool init();
    
    
    CREATE_FUNC(SerifLayer);

    CC_SYNTHESIZE(float, scrollStartDelay, ScrollStartDelay);
    CC_SYNTHESIZE(CCPoint, basePos, BasePos);
    CC_SYNTHESIZE(CCSize, serifSize, SerifSize);
    CC_SYNTHESIZE(CCSize, serifContentSize, SerifContentSize);
    CC_SYNTHESIZE(CCSprite*, serif, Serif);
    CC_SYNTHESIZE(CCScrollView*, scroll, Scroll);
    
    void setTextString(const char* text);
    void setTextString(CCLabelTTF* label);
    void setTextQueue(const char* text);
    
    void removeSerif(float delay);
    
private:
    int scrolls;
    float totalDelta;
    void initLayout();
    void update(float delta);
    
    bool isSerifNow;
    std::string serifStr;
    void createSerif();
    void serifUpdateReady();
    void serifUpdate(float delta);
};

#endif /* defined(__Ranotan2D__SerifLayer__) */
