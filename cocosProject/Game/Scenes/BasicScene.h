#pragma once

#include <cocos2d.h>
#include <cfloat>

class MyLayer;
class BasicScene : public cocos2d::Scene {
public:
    virtual bool init() override;
    virtual bool initWithPhysics();
    virtual void onBack();
    virtual void setPosition(const cocos2d::Vec2 &position) override;
    
    virtual void onAppPause() { }
    virtual void onAppResume() { }
    
    cocos2d::Vec2 getRelativePos(cocos2d::Vec2 pos);
    cocos2d::Vec2 getAbsolutePos(cocos2d::Vec2 pos);
    
protected:
    
    cocos2d::Vec2 getSceneMousePos(cocos2d::Event* event);
    
    virtual inline void onMouseMove(cocos2d::Vec2) { }
    virtual inline void onMouseDown(cocos2d::Vec2) { }
    virtual inline void onMouseUp(cocos2d::Vec2) { }
    virtual inline void onTangentChange(float angle) { }

    bool initBasic();
    void initMouseCursor();
    
    void update(float dt) override;
    
    BasicScene() {}
    ~BasicScene() {}
    cocos2d::Size _size;
    
    friend MyLayer;
    
    float _tangent;
    float _prevTangent = FLT_MAX;
    
};