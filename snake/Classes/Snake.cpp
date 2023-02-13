//
//  Snake.cpp
//  snake
//
//  Created by Sunny Yu on 20/12/2016.
//
//

#include "Snake.h"
#include <iostream>
using namespace std;
using namespace cocos2d::network;

bool Snake::init(){
    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    auto midPoint = Point(size.width/2 + origin.x ,size.height/2 + origin.y);
    this->matchId = 0;
    numberOfSprite = 1;
    addLength = 0;
    tShortSum=0;
    tSum = 0.0;
    //cout<<"**"<<size.width<<" "<<size.height<<endl;
    //numberOfPoint = background->getNumberOfPoint();
    snakeBody.clear();
    for (int i = 0; i <= 50; i++)
    {
        snakeBody.push_back(Vec2(midPoint.x,midPoint.y + i * 3));
    }
    auto head = Sprite::create("NewCircle.png");
    head->setAnchorPoint(Vec2(0.5,0.5));
    head->setScale(SNAKE_HEAD_SCALE);
    head->setPosition(Point(snakeBody[0]));
    head->setTag(0);
    this->addChild(head);
    for(int i = 3; i < snakeBody.size(); i+=3){
        auto body = Sprite::create("NewCircle.png");
        body->setScale(SNAKE_BODY_SCALE);
        body->setAnchorPoint(Vec2(0.5,0.5));
        body->setPosition(standardizePoint(snakeBody[i]));
        body->setTag(numberOfSprite);
        numberOfSprite++;
        this->addChild(body);
        
    }
    this->request = new HttpRequest();
    
    this->scheduleUpdate();
    
    return true;
}

Point Snake::standardizePoint(Point pt)
{
    auto midPoint = Point(size.width/2 + origin.x ,size.height/2 + origin.y);
    auto delta = pt - midPoint;
    delta *= size.width/STD_WIDTH;
    return midPoint + delta;
}


void Snake::update(float dt)
{
    tSum += dt;
    bool isAcc = joystick->getIsAcc();
    float period = 0.05;
    
    if(snakeBody.size() < 30){
        isAcc = false;
    }
    
    if(tShortSum < 0.3 && isAcc)
    {
        tShortSum += dt;
    }else if(tShortSum > 0.3){
        tShortSum = 0.0;
        addLength = -1;
    }
    
    
    if(isAcc)
    {
        period = 0.02;
    }
    if(tSum < period)
    {
        return;
    }
    tSum = 0;
    
    //cout << "(" << enemySnakeVec->size() << ")" << endl;
    for(int j = 0; j < enemySnakeVec->size(); j++){
        auto enemySnake = this->getParent()->getChildByTag(j+10);
        
        if(enemySnake && enemySnake->getChildByTag(0)){
            auto enemyHeadPosition = enemySnake->getChildByTag(0)->getPosition();
            for(int i = 1; i < numberOfSprite; i++){
                auto distance = enemyHeadPosition.getDistance(this->getChildByTag(i)->getPosition());
                if (distance < 10){
                    //cout << "*"<< j << "*" <<endl;
                    (*enemySnakeVec)[j]->killed();
                    return;
                }
            }
            auto enemySnake = enemySnakeVec[0][j];
            auto headPosition = this->getChildByTag(0)->getPosition();
            for(int i = 1; i < enemySnake->numberOfSprite; i++){
                auto distance = headPosition.getDistance(enemySnake->getChildByTag(i)->getPosition());
                if (distance < 10){
                    die();
                    return;
                }
            }
        }
    }
    
    
    //auto background = this->getParent()->getChildByTag(9);
    for(int i = 1; i < background->getNumberOfPoint(); i++){
        if(background->getChildByTag(i)){
            auto pointPosition = background->getChildByTag(i)->getPosition();
            for(int j = 1; j < numberOfSprite; j++){
                auto distance = pointPosition.getDistance(this->getChildByTag(j)->getPosition());
                if (distance < 10){
                    background->getChildByTag(i)->removeFromParent();
                    addLength = 8;
                    break;
                }
                
            }
        }
    }
    
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto direction = joystick->getDirection();
    if(addLength>0){
        snakeBody.push_back(Vec2(0,0));
        addLength--;
    }
    if(addLength < 0){
        snakeBody.pop_back();
        addLength++;
    }
    for(int i = snakeBody.size() - 1; i > 0  ; i--)
    {
        snakeBody[i] = snakeBody[i - 1] - direction * 3;
    }
    //snakeBody[snakeBody.size()-1];
    for(int i = 0; i <  snakeBody.size(); i+=3)
    {
        if(numberOfSprite<= i/3){
            auto body = Sprite::create("NewCircle.png");
            body->setScale(0.1);
            body->setAnchorPoint(Vec2(0.5,0.5));
            body->setPosition(standardizePoint(snakeBody[i]));
            body->setTag(numberOfSprite++);
            this->addChild(body);
        }
        this->getChildByTag(i/3)->setPosition(standardizePoint(snakeBody[i]));
    }
    if (this->getChildByTag(snakeBody.size()/3))
    {
        //auto position = this->getChildByTag(snakeBody.size()/3)->getPosition();
        //this->background->addPoint(position);
        this->getChildByTag(snakeBody.size()/3)->removeFromParent();
        numberOfSprite--;
    }
    
    
    auto position = background->getChildByTag(0)->getPosition();
    auto deltaPosition = new Vec2(position.x - size.width/2 - origin.x,position.y - size.height/2 - origin.y);
        
    if (deltaPosition->x - direction.x*3 > -470 && deltaPosition->x - direction.x*3 < 470)
    {
        for(int i = 0; i < background->getNumberOfPoint(); i++){
            if(background->getChildByTag(i)){
                auto position = background->getChildByTag(i)->getPosition();
                background->getChildByTag(i)->setPosition(position.x - direction.x*3, position.y);
            }
        }
    }else{
        die();
    }
    if (deltaPosition->y - direction.y*3 > -260 && deltaPosition->y - direction.y*3 < 260)
    {
        for(int i = 0; i < background->getNumberOfPoint(); i ++){
            if(background->getChildByTag(i)){
                auto position = background->getChildByTag(i)->getPosition();
                background->getChildByTag(i)->setPosition(position.x,position.y - direction.y*3);
            }
        }
    }else{
        die();
    }
    
    if(0 != this->matchId){
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        
        document.AddMember("player","iphone", allocator);
        document.AddMember("match", this->matchId, allocator);
        
        rapidjson::Value posList(rapidjson::kArrayType);
        
        for(int i=0; i<snakeBody.size(); i++){
            rapidjson::Value object(rapidjson::kObjectType);
            object.AddMember("x", snakeBody[i].x, allocator);
            object.AddMember("y", snakeBody[i].y, allocator);
            posList.PushBack(object, allocator);
        }
        
        document.AddMember("pos", posList, allocator);
        
        rapidjson::StringBuffer jsonBuffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(jsonBuffer);
        document.Accept(writer);
        
        string postDataStr = jsonBuffer.GetString();
        
        cout << postDataStr;
        
        // 这是一个专门供开发者测试发送请求的服务器地址，post方法会返回POST请求发送的数据
        auto request = this->request;
        request -> setUrl("http://127.0.0.1:8000/position/");
        
        // 设置请求类型，可以选择GET
        request -> setRequestType(HttpRequest::Type::POST);
        // 设置请求完成之后的响应方法
        request -> setResponseCallback(CC_CALLBACK_2(Snake::onRequestCompleted, this));
//        auto postData = jsonBuffer.GetString();
        vector<string> headers;
        headers.push_back("Content-Type: application/json; charset=utf-8");
        // 设置请求头，如果数据为键值对则不需要设置
        request -> setHeaders(headers);
        // 传入发送的数据及数据ch n g
        const char* postData = postDataStr.c_str();
        request -> setRequestData(postData ,strlen(postData));
        request -> setTag("UPLOAD POSITION");
        HttpClient::getInstance() -> send(request);
    }
    //updateSnake();
}


void Snake::onRequestCompleted(HttpClient* sender, HttpResponse* response){
    if(!response){
        return;
    }
    log("%s completed", response -> getHttpRequest() -> getTag());
    int statusCode = response -> getResponseCode();
    log("response code: %d", statusCode);
    if(!response -> isSucceed()){
        log("response failed");
        log("error buffer: %s", response -> getErrorBuffer());
        return;
    }
}

void Snake::setMatchId(int matchId){
    this->matchId = matchId;
}

void Snake::setJoystick(Joystick *joystick)
{
    this->joystick = joystick;
    
}

void Snake::setBackground(BackgroundLayer* backgroundLayer, vector<EnemySnake*> *enemySnakeVec){
    this->background = backgroundLayer;
    this->enemySnakeVec = enemySnakeVec;
}

void Snake::die(){
    this->background->getParent()->unscheduleUpdate();
    this->unscheduleUpdate();
    this->removeAllChildren();
    for(int i = 0; i < enemySnakeVec->size(); i++){
        auto s = (*enemySnakeVec)[i];
        //if(s->numberOfSprite > 0){
            s->unscheduleUpdate();
            s->removeAllChildren();
            s->removeFromParent();
        //}
    }
    enemySnakeVec->clear();
    auto color = LayerColor::create(Color4B(30, 0, 0, 200));
    this->addChild(color);
    auto scoreLabel = Label::createWithSystemFont("Score: " + std::to_string(snakeBody.size()) , "courier", 20);
    auto size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto midPoint = Point(size.width/2 + origin.x ,size.height/2 + origin.y);
    scoreLabel->setPosition(midPoint);
    this->addChild(scoreLabel);
    this->background->clearPoint();
    //this->background->restart();
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [this, midPoint](Touch *touch,Event *event)
    {
        this->removeAllChildren();
        this->background->getChildByTag(0)->setPosition(midPoint);
        this->init();
        this->background->getParent()->scheduleUpdate();
        Director::getInstance()->getEventDispatcher()->removeEventListener(touchListener);
        return true;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    this->request -> release();
}
