//
//  CCAsyncSprite.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/04/11.
//
//

#ifndef __Ranotan2D__CCAsyncSprite__
#define __Ranotan2D__CCAsyncSprite__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CCAsyncSprite : public CCSprite
{
public:
    CCAsyncSprite();
    virtual ~CCAsyncSprite();
    virtual bool init(const char* filename);
    virtual bool init(CCTexture2D* texture);
    
    static CCAsyncSprite* create(const char* filename);
    static CCAsyncSprite* createWithTexture(CCTexture2D* texture);

private:
    void loadedImage(cocos2d::CCObject* object);
};

#endif /* defined(__Ranotan2D__CCAsyncSprite__) */
