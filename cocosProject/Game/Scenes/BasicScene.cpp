#include "Managers/SettingsManager.h"
#include "Managers/LocalizationManager.h"
#include "Managers/ScenesManager.h"

#include "Scenes/BasicScene.h"

#include "Utils.h"


using namespace cocos2d;


class MyLayer : public LayerColor {
    
public:
    CREATE_FUNC(MyLayer);
    
    bool init() {
        if(!LayerColor::initWithColor(Color4B::WHITE))
            return false;
        
        auto listener2 = EventListenerTouchOneByOne::create();
        listener2->onTouchBegan = [this](Touch* touch, Event* event) {
            auto pos = convertTouchToNodeSpace(touch);
            if(_basicScene)
                _basicScene->onMouseDown(pos);
            return true;
        };
        
        listener2->onTouchMoved = [this](Touch* touch, Event* event) {
            auto pos = convertTouchToNodeSpace(touch);
            if(_basicScene)
                _basicScene->onMouseMove(pos);
        };
        
        listener2->onTouchEnded = [this](Touch* touch, Event* event) {
            auto pos = convertTouchToNodeSpace(touch);
            if(_basicScene)
                _basicScene->onMouseUp(pos);
        };
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
        return true;
    }
    
    BasicScene* _basicScene = nullptr;
    
};

bool BasicScene::init() {
    if(!Scene::init())
        return false;
    return initBasic();
}

bool BasicScene::initBasic() {
    //_size = SettingsManager::getInstance()->getWinSize();
    _size = Director::getInstance()->getVisibleSize();
    setPosition(Vec2::ZERO);
    
    auto lc = MyLayer::create();
    lc->_basicScene = this;
    addChild(lc, 1);
    
#ifdef LINUX
    //initMouseCursor();
#endif
    
    _tangent = 0;
    
    Device::setAccelerometerEnabled(true);
    auto ela = EventListenerAcceleration::create([this](Acceleration* a, Event*) {
        float prevTangent = _tangent;
        _tangent = a->x * 90;
        //CCLOG("tangent x: %f", a->x);
        if(_tangent < -90)
            _tangent = -90;
        if(_tangent > 90)
            _tangent = 90;
        if(prevTangent != _tangent)
            onTangentChange(_tangent);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(ela, this);
    scheduleUpdate();
    return true;
}

bool BasicScene::initWithPhysics() {
    if(!Scene::initWithPhysics())
        return false;
    return initBasic();
}

void BasicScene::setPosition(const cocos2d::Vec2& position) {
    Scene::setPosition(
        position + Director::getInstance()->getWinSize() - _size
    );
}

void BasicScene::onBack() {
    ScenesManager::getInstance()->toggleExitMessage();
}

void BasicScene::initMouseCursor() {
    
    static Vec2 prevPos;
    
    auto sp = Sprite::create("circle.png");
    sp->setColor(Color3B::YELLOW);
    sp->setPosition(prevPos);
    sp->setOpacity(210);
    addChild(sp, 999);
    
    auto listener1 = EventListenerMouse::create();
    sp->setVisible(false);
    listener1->onMouseMove = [sp, this](Event* event) {
        auto pos = getSceneMousePos(event);
        sp->setPosition(pos);
        sp->setVisible(true);
    };
    
    listener1->onMouseDown = [sp, this](Event* event) {
        sp->setColor(Color3B::RED);
    };
    
    listener1->onMouseUp = [sp, this](Event* event) {
        sp->setColor(Color3B::YELLOW);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);
    
}

Vec2 BasicScene::getSceneMousePos(Event* event) {
    EventMouse* e = (EventMouse*)event;
    auto pos = Vec2(e->getCursorX(), e->getCursorY());
    return pos;
}

Vec2 BasicScene::getRelativePos(Vec2 pos) {
    return Vec2(pos.x / _size.width, pos.y / _size.height);
}

Vec2 BasicScene::getAbsolutePos(Vec2 pos) {
    return Vec2(pos.x * _size.width, pos.y * _size.height);
}

void BasicScene::update(float dt) {
    Scene::update(dt);
    
    
}

