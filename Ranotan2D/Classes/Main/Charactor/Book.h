//
//  Book.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/06.
//
//

#ifndef __Ranotan2D__Book__
#define __Ranotan2D__Book__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum BookStatus
{
    BookNomal   = 1,
    BookTap     = 2,
};


class Book : public CCSprite
{
public:
    Book();
    virtual ~Book();
    virtual bool init(const char* filename);
    
    static Book* create(const char* filename);
    void setStatus(BookStatus status);
    CC_SYNTHESIZE(float, deg, Deg);
    CC_SYNTHESIZE(float, delayDeg, DelayDeg);
    //何秒で１周するか。
    CC_SYNTHESIZE(int, tag, Tag);
    CC_SYNTHESIZE(float, speed, Speed);
    CC_SYNTHESIZE(int, zOrderSetting, ZOrderSetting);
    CC_SYNTHESIZE(float, scale, Scale);
    CC_SYNTHESIZE(float, posX, PosX);
    CC_SYNTHESIZE(float, posY, PosY);
    CC_SYNTHESIZE(float, widthX, WidthX);
    CC_SYNTHESIZE(float, widthY, WidthY);
    CC_SYNTHESIZE(float, basePosX, BasePosX);
    CC_SYNTHESIZE(float, basePosY, BasePosY);
    
private:
    BookStatus status;
    void update(float delta);
    void changeAnimation();
};

#endif /* defined(__Ranotan2D__Book__) */
