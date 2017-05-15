//
//  OpacityLayer.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/08/19.
//
//

#include "OpacityLayer.h"


OpacityLayer::OpacityLayer()
{
}
OpacityLayer::~OpacityLayer()
{
}


OpacityLayer* OpacityLayer::create()
{
    OpacityLayer *pRet = new OpacityLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        pRet->m_nOpacity = 255;
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}
GLubyte OpacityLayer::getOpacity(void){
    return m_nOpacity;
}
void OpacityLayer::setOpacity(GLubyte opacity)
{
    m_nOpacity = opacity;
    CCObject* child;
    CCARRAY_FOREACH(m_pChildren, child)
    {
        CCNode* pNode = (CCNode*)child;
        cocos2d::CCRGBAProtocol *pRGBAProtocol = dynamic_cast<cocos2d::CCRGBAProtocol*>(pNode);
        if (pRGBAProtocol)
        {
            pRGBAProtocol->setOpacity(opacity);
        }
    }
}