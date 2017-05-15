//
//  TextureFactory.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/22.
//
//

#ifndef __Ranotan2D__TextureFactory__
#define __Ranotan2D__TextureFactory__

#import <cocos2d.h>
#import "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define SIZE_TEXTURES (9)
#define SIZE_UPPER_RANGE (4)
#define SIZE_LOWER_RANGE (4)

class TextureFactory :public CCObject
{
public:

    CC_SYNTHESIZE(int, index, Index);
    CC_SYNTHESIZE(int, startIndex, StartIndex);
    CC_SYNTHESIZE(int, endIndex, EndIndex);
    CC_SYNTHESIZE(int, currentIndex, CurrentIndex);
    CC_SYNTHESIZE(CCArray*, textures, Textures);
    CC_SYNTHESIZE(CCArray*, texturesTemp, TexturesTemp);
    CC_SYNTHESIZE_RETAIN(CCArray*, itemList, ItemList);
    
    TextureFactory();
    virtual ~TextureFactory();
    virtual bool init();
    CREATE_FUNC(TextureFactory);
    
    static TextureFactory* shardInstance();
    
    void initTextures(CCArray* list, int idx);
    
private:
    void createTextures();
    void loadedImage(CCObject* object);
    void resetTextures();
    
};

#endif /* defined(__Ranotan2D__TextureFactory__) */
