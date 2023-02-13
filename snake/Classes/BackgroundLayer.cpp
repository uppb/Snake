//
//  BackgroundLayer.cpp
//  snake
//
//  Created by Sunny Yu on 2016/12/3.
//
//

#include "BackgroundLayer.h"
#include "cocos2d.h"
#include <iostream>
USING_NS_CC;
using namespace std;
bool BackgroundLayer::init()
{
    auto size = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    position = Vec2(0,0);
    sprite = Sprite::create("NewBackground.png");
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->setPosition(origin.x + size.width/2,origin.y + size.height/2);
    sprite->setTag(0);
    numberOfPoint = 1;
    this->addChild(sprite);
    //this->scheduleUpdate();
    return true;
    
}


void BackgroundLayer::update(float dt)
{
    position = this->getChildByTag(0)->getPosition();
    
}

void BackgroundLayer::addPoint(Point position)
{
    point = Sprite::create("NewCircle.png");
    point->setScale(0.1);
    point->setAnchorPoint(Vec2(0.5, 0.5));
    point->setPosition(position);
    point->setTag(numberOfPoint);
    this->addChild(point);
    numberOfPoint++;
}



int BackgroundLayer::getNumberOfPoint(){
    return numberOfPoint;
}

void BackgroundLayer::clearPoint(){
    for(int i = 1; i < numberOfPoint; i++){
        auto point = this->getChildByTag(i);
        if(!point) continue;
        point->removeFromParent();
    }
    numberOfPoint = 1;
}
