//
//  BackgroundLayer.hpp
//  snake
//
//  Created by Sunny Yu on 2016/12/3.
//
//

#ifndef BackgroundLayer_h
#define BackgroundLayer_h

#include "cocos2d.h"
#include "Joystick.h"

class BackgroundLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(BackgroundLayer);
    Vec2 getBackground();
    void addPoint(Point position);
    void clearPoint();
    int getNumberOfPoint();
    void restart();
private:
    Vec2 position;
    void update(float dt);
    int width;
    int height;
    Sprite* sprite;
    int numberOfPoint;
    Sprite* point;
};

#endif /* BackgroundLayer_hpp */
