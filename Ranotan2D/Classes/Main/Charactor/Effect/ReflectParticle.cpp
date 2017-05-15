//
//  ReflectParticle.cpp
//  Ranotan2D
//
//  Created by talestaka on 2015/04/16.
//
//

#include "ReflectParticle.h"

Reflectparticle::Reflectparticle()
{
    speedX = 0;
    speedY = 0;
    seekX = 1;
    seekY = 1;
    move = false;
}

Reflectparticle::~Reflectparticle()
{
    speedX = 0;
    speedY = 0;
    seekX = 1;
    seekY = 1;
    move = false;
    CCLOG("%s", "Reflectparticle Destractor");
}

bool Reflectparticle::init(CCPoint startPoint)
{
    this->setPosition(startPoint);
    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("ListChangeEffect.plist");
    particle->setPosition(CCPointZero);
    particle->setDuration(2.0);
    particle->setAutoRemoveOnFinish(true);
    addChild(particle, 0, 0);
    
    speedX = 1 + arc4random() % 5;
    speedY = 1 + arc4random() % 5;
    if ( arc4random()%2==0 ) {
        seekX=1;
    } else {
        seekX=-1;
    }
    if ( arc4random()%2==0 ) {
        seekY=1;
    } else {
        seekY=-1;
    }
    
    scheduleUpdate();
    
    return true;
}

Reflectparticle* Reflectparticle::create(CCPoint startPoint)
{
    Reflectparticle* pRet = new Reflectparticle();
    if (pRet && pRet->init(startPoint)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        return NULL;
    }
}

void Reflectparticle::update(float delta)
{
    if ( !move ) return;
    CCPoint point =getPosition();
    //位置判定
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if ( point.x < 0.0 ) {
        seekX = 1;
        speedX = 3 + arc4random() % 5;
    }
    if ( point.x > winSize.width ) {
        seekX = -1;
        speedX = 3 + arc4random() % 5;
    }
    if ( point.y < 0.0 ) {
        seekY = 1;
        speedY = 3 + arc4random() % 5;
    }
    if ( point.y > winSize.height ) {
        seekY = -1;
        speedY = 3 + arc4random() % 5;
    }
    setPosition(ccp(point.x +(speedX*seekX), point.y + (speedY*seekY)));
}

void Reflectparticle::doMove()
{
    
    CCParticleSystemQuad *particle = CCParticleSystemQuad::create("ListChangeEffect.plist");
    particle->setPosition(CCPointZero);
    particle->setTotalParticles(150);
    particle->setDuration(kCCParticleDurationInfinity);
    particle->setAutoRemoveOnFinish(true);
    addChild(particle, 0, 0);

    move=true;
}


