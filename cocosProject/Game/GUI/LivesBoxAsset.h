#pragma once

#include <cocos2d.h>

class LivesBoxAsset : public cocos2d::Node {

public:
    CREATE_FUNC(LivesBoxAsset);
    virtual void update(float dt) override;
    
private:
    
    /* DATA */
    const int ELEMENTS_NUMBER = 3;
    
    /* ELEMENTS */
    cocos2d::Node* _back;
    cocos2d::Label* _numberLabel;
    cocos2d::Label* _timeToNextLifeLabel;
    cocos2d::Label* _timeToNextLifeTitleLabel;
    
    /* INITS */
    bool init() override;
    void initBack();
    void initCounter();
    void initIcons();
    
    
    /* HANDLERS */
    void onFull();
    
    cocos2d::Size _size;
    
};