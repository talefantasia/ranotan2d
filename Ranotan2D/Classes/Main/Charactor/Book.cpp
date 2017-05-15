//
//  Book.cpp
//  Ranotan2D
//
//  Created by Takahashi Hiroyuki on 2014/03/06.
//
//

#include "Book.h"

Book::Book()
{
    status=BookNomal;
    deg = 0.0;
    delayDeg = 0.0;
    speed = 2.0;
    zOrderSetting = 0;
    scale = 1.0;
    posX = 0.0;
    posY = 0.0;
    widthX = 100.0;
    widthY = 20.0;
    basePosX = 400.0;
    basePosY = 400.0;
}

Book::~Book()
{
    status=BookNomal;
    deg = 0.0;
    delayDeg = 0.0;
    speed = 0.0;
    zOrderSetting = 0;
    scale = 1.0;
    posX = 0.0;
    posY = 0.0;
    widthX = 100.0;
    widthY = 20.0;
    basePosX = 400.0;
    basePosY = 400.0;
}

bool Book::init(const char *filename)
{
    if ( !CCSprite::initWithFile(filename) ) {
        return false;
    }
    changeAnimation();
    scheduleUpdate();
    return true;
}

Book* Book::create(const char *filename)
{
    Book* pRet = new Book();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void Book::changeAnimation()
{
    switch (status) {
        case BookNomal:
            break;
            
        default:
            break;
    }
}

void Book::update(float delta)
{
    //スピードから回転角度を算出
    deg = deg + 360.0 * delta/speed;
    posX = basePosX + widthX * sin(CC_DEGREES_TO_RADIANS(deg));
    posY = basePosY + widthY * cos(CC_DEGREES_TO_RADIANS(deg+delayDeg));
    if ( cos(CC_DEGREES_TO_RADIANS(deg))>=0 ) {
       this->setZOrder(zOrderSetting);
    } else {
        this->setZOrder(zOrderSetting
                        -3);
    }
    scale = cos(CC_DEGREES_TO_RADIANS(deg))*0.15+0.85;
    this->setPosition(ccp(posX, posY));
    this->setScaleX(scale);
    this->setScaleY(scale);
}