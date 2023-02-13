//
//  GameScene.hpp
//  snake
//
//  Created by Sunny Yu on 2016/12/3.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "BackgroundLayer.h"
#include "Joystick.h"
#include "Snake.h"
#include "EnemySnake.h"
#include <vector>
#include "network/HttpClient.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

class GameScene : public cocos2d:: Layer
{
public:
    virtual bool init();
    
    static cocos2d:: Scene* createScene();
    
    vector<EnemySnake*> enemySnakeVec;
    
    CREATE_FUNC(GameScene);
private:
    Snake* snake;
    void update(float dt);
    int numberOfEnemy;
    int matchId;
    void getAllPositionByMatch();
    float sumT;
    BackgroundLayer *backgroundLayer;
    void onRequestCompleted(cocos2d::network::HttpClient* sender,cocos2d::network::HttpResponse* response);
};



#endif /* GameScene_hpp */
