//
//  OpacityLayer.h
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/08/19.
//
//

#ifndef __Ranotan2D__OpacityLayer__
#define __Ranotan2D__OpacityLayer__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class OpacityLayer : public cocos2d::CCLayerRGBA
{
public:
    OpacityLayer();
    ~OpacityLayer();
    CC_PROPERTY(GLubyte, m_nOpacity, Opacity)
public:
    static OpacityLayer *create(void);
};

#endif /* defined(__Ranotan2D__OpacityLayer__) */
