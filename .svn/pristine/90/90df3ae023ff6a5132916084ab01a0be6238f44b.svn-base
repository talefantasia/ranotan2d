//
//  ImageAsyncSprite.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2013/12/14.
//
//

#ifndef __Ranotan2D__ImageAsyncSprite__
#define __Ranotan2D__ImageAsyncSprite__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ImageAsyncSprite : public CCSprite
{
public:
    CCSize imageSize;
    const char* name;
    ImageAsyncSprite();
    virtual ~ImageAsyncSprite();
    virtual bool init(const char* fileName, const char* url, CCSize size);
    static ImageAsyncSprite* create(const char* fileName, const char* url, CCSize size);
    void onHttpRequestCompleted(CCNode *sender, void *data);
    
private:
    void getHttpAsync(const char* url);
};

#endif /* defined(__Ranotan2D__ImageAsyncSprite__) */
