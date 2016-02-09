#pragma once

#include <cocos2d.h>

class TestScene : public cocos2d::Scene {

public:
    
    CREATE_FUNC(TestScene);
    
    virtual bool init() override;
    
    /* BUTTON TEST */
    void initButtonTest();
    
    /* HSL CONVERSION TEST */
    void initHSLTest();
    
    /* SHADER TEST */
    void initShadingTest();
    void graySprite(cocos2d::Sprite*);
    void genericSprite(cocos2d::Sprite*);
     
    /* MESSAGES */
    void initMessagesTest();
    
    /* 3D TESTS */
    void init3DTest();
    
    /* COLLISION TEST */
    void initCollisionTest();
    
    /* LIGHT 3D TEST */
    void initLight3DTest();
    
    /* ANIMATION 3D TEST */
    void initAnimation3DTest();
    
    /* CLIPPING TEST */
    void initClippingTest();
    void initClippingTestBackground();
    void initClipingTestCamera();
    

private:
    cocos2d::Size _size;
    

    TestScene() { }
    TestScene(const TestScene& orig) { }
    virtual ~TestScene() { }
};

